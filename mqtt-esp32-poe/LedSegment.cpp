#include <EEPROM.h>
#include "LedSegment.h"

void LedSegment::begin(int ledOffset, int ledCount, String subtopic, int memoryAddress, AddressableLeds* leds, MqttPubSub* mqtt) {
  Serial.print("Init LedSegment - ledOffset: " + ledOffset);
  Serial.print(" | ledCount: " + ledCount);
  Serial.print(" | subtopic: " + subtopic);
  Serial.println(" | memoryAddress: " + memoryAddress);

  _ledOffset = ledOffset;
  _ledCount = ledCount;
  _subtopic = subtopic;
  _subtopicLevel = subtopic + "/level";
  _subtopicAnimation = subtopic + "/animation";
  _subtopicAnimationIndex = subtopic + "/animations_possible";
  _subtopicSpeed = subtopic + "/speed";
  _mqtt = mqtt;
  _leds = leds;
  _memoryAddress = memoryAddress;
  _memoryAddressLevel = memoryAddress;
  _memoryAddressAnimation = memoryAddress + 1;
  _memoryAddressSpeed = memoryAddress + 2;

  _mqtt->subscribe(_subtopicLevel);
  _mqtt->subscribe(_subtopicAnimation);
  _mqtt->subscribe(_subtopicSpeed);
  prepareAnimations();
  restoreFromEepromAndPublish();
}

void LedSegment::prepareAnimations() {
  Animation* animationNone = new AnimationNone();
  Animation* animationFade = new AnimationFade();
  Animation* animationFlicker = new AnimationFlicker();
  Animation* animationMovingFadeUp = new AnimationMovingFadeUp();
  Animation* animationMovingFadeDown = new AnimationMovingFadeDown();
 
  _animations[ANIMATION_NONE] = animationNone;
  _animations[ANIMATION_FADE] = animationFade;
  _animations[ANIMATION_FLICKER] = animationFlicker;
  _animations[ANIMATION_MOVINGFADEUP] = animationMovingFadeUp;
  _animations[ANIMATION_MOVINGFADEDOWN] = animationMovingFadeDown;

  for (int i=0; i<_numAnimations; i++) {
    _animations[i]->begin(_ledOffset, _ledCount, _leds);
  }
}

void LedSegment::restoreFromEepromAndPublish() {
  byte eepromLevel = EEPROM.read(_memoryAddressLevel);
  byte eepromAnimation = EEPROM.read(_memoryAddressAnimation);
  byte eepromSpeed = EEPROM.read(_memoryAddressSpeed);
  setLevel(eepromLevel);
  setAnimation(eepromAnimation);
  setAnimationSpeed(eepromSpeed);
  _mqtt->publishState(_subtopicLevel, String(eepromLevel));
  _mqtt->publishState(_subtopicAnimation, number2AnimationName(eepromAnimation));
  _mqtt->publishState(_subtopicAnimationIndex, getPossibleAnimationsJsonArray());
  _mqtt->publishState(_subtopicSpeed, String(eepromSpeed));
}

void LedSegment::resubscribe() {
  _mqtt->resubscribe(_subtopicLevel);
  _mqtt->resubscribe(_subtopicAnimation);
  _mqtt->resubscribe(_subtopicSpeed);
}

String LedSegment::getPossibleAnimationsJsonArray() {
  String animationsPossible = String("[ ");
  for (int i=0; i<_numAnimations; i++) {
    animationsPossible.concat("\"");
    animationsPossible.concat(_animations[i]->getName());
    animationsPossible.concat("\"");
    if (i < _numAnimations - 1)
      animationsPossible.concat(", ");
  }
  animationsPossible.concat(" ]");
  return animationsPossible;
}

String LedSegment::number2AnimationName(byte number) {
  if (number < 0 || number >= _numAnimations)
    number = 0;

  return _animations[number]->getName();
}

byte LedSegment::animationName2Number(String animationName) {
  for (int i=0; i<_numAnimations; i++) {
    if (String(_animations[i]->getName()).equalsIgnoreCase(animationName))
      return i;
  }
  return 0;
}

void LedSegment::loop() {
  if (_animationStep == 0 && !_leds->powerIsOn())
    _leds->powerOn();

  if (_animationStep < 0 || !_leds->powerOnDelayIsOver() || waitingForNextFrame())
    return;

  _animations[_activeAnimation]->doAnimationStep(_animationStep);
 
  if (!_animations[_activeAnimation]->isRunning()) {
    _animationStep = -1;
    return;
  }

  _animationStep++;
  _lastAnimationStepMicros = micros();
}

byte LedSegment::strToPercentage(String newValueString) {
  int newValueInt = newValueString.toInt();
  if (newValueInt > 100)
    return 100;
  else if (newValueInt < 0)
    return 0;
  else 
    return (byte)newValueInt;
}

bool LedSegment::waitingForNextFrame() {
  return micros() - _lastAnimationStepMicros < ceil((100.1 - (float)_speed) * (100.1 - (float)_speed) * 10.0);
}

void LedSegment::callback(String receivedMessageTopic, String newValueString) {
  if(_ledCount == 0)
    return;

  byte newValue = 0;

  if (receivedMessageTopic.endsWith(_subtopicLevel)) {

    newValue = strToPercentage(newValueString);

    if (_level == newValue)
      return;

    if(EEPROM.read(_memoryAddressLevel) != newValue) {
      EEPROM.write(_memoryAddressLevel, newValue);
      EEPROM.commit();
    }
    _mqtt->publishState(_subtopicLevel, String(newValue));
    setLevel(newValue);
  } else if (receivedMessageTopic.endsWith(_subtopicAnimation)) {

    newValue = animationName2Number(newValueString);

    if (_animation == newValue)
      return;

    if(EEPROM.read(_memoryAddressAnimation) != newValue) {
      EEPROM.write(_memoryAddressAnimation, newValue);
      EEPROM.commit();
    }
    _mqtt->publishState(_subtopicAnimation, number2AnimationName(newValue));
    setAnimation(newValue);
  } else if (receivedMessageTopic.endsWith(_subtopicSpeed)) {

    newValue = strToPercentage(newValueString);

    if (_speed == newValue)
      return;

    if(EEPROM.read(_memoryAddressSpeed) != newValue) {
      EEPROM.write(_memoryAddressSpeed, newValue);
      EEPROM.commit();
    }
    _mqtt->publishState(_subtopicSpeed, String(newValue));
    setAnimationSpeed(newValue);
  } else
    return;

  Serial.print(receivedMessageTopic);
  Serial.print(F(" = "));
  Serial.println(newValue);
}

void LedSegment::setLevel(byte newValue) {
  _animations[_animation]->start(newValue, _level);

  _level = newValue;
  
  // This switches/triggers the new animation start
  _activeAnimation = _animation;
  _animationStep = 0;
}

void LedSegment::setAnimation(byte newValue) {
  _animation = newValue;
}

void LedSegment::setAnimationSpeed(byte newValue) {
  _speed = newValue;
}
