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
  _animationNone.begin(_ledOffset, _ledCount, _leds);
  _animationFade.begin(_ledOffset, _ledCount, _leds);
  
  _animationNames[ANIMATION_NONE] = _animationNone.getName();
  _animationNames[ANIMATION_FADE] = _animationFade.getName();
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

String LedSegment::getPossibleAnimationsJsonArray() {
  String animationsPossible = String("[ ");
  for (int i=0; i<_numAnimations; i++) {
    animationsPossible.concat("\"");
    animationsPossible.concat(_animationNames[i]);
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

  return _animationNames[number];
}

byte LedSegment::animationName2Number(String animationName) {
  for (int i=0; i<_numAnimations; i++) {
    if (String(_animationNames[i]).equalsIgnoreCase(animationName))
      return i;
  }
  return 0;
}


void LedSegment::loop() {
  if (_animationStep > 100)
    _animationStep = -1;
  
  if (_animationStep < 0 || millis() - _lastAnimationStepMillis < ceil(sqrt(100 - _speed) * 2.0))
    return;

  bool animationIsRunning = false;
  
  switch(_activeAnimation) {
    case ANIMATION_FADE:
      _animationFade.doAnimationStep(_animationStep);
      animationIsRunning = _animationFade.isRunning();
      break;
    case ANIMATION_NONE:
    default:
      _animationNone.doAnimationStep();
      animationIsRunning = _animationNone.isRunning();
      break;
  }

  if (!animationIsRunning) {
    _animationStep = -1;
    return;
  }

  _animationStep++;
  _lastAnimationStepMillis = millis();
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
  _level = newValue;

  switch(_animation) {
    case ANIMATION_FADE:
      _animationFade.start(_level);
      break;
    case ANIMATION_NONE:
    default:
      _animationNone.start(_level);
      break;
  }
  
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
