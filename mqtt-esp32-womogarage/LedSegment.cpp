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
  _animationNames[0] = _animationNone.getName();
}

void LedSegment::restoreFromEepromAndPublish() {
  byte eepromLevel = EEPROM.read(_memoryAddressLevel);
  byte eepromAnimation = EEPROM.read(_memoryAddressAnimation);
  byte eepromSpeed = EEPROM.read(_memoryAddressSpeed);
  setLevel(eepromLevel);
  setAnimation(eepromAnimation);
  setAnimationSpeed(eepromSpeed);
  _mqtt->publishState(_subtopicLevel, String(eepromLevel));
  _mqtt->publishState(_subtopicAnimation, String(eepromAnimation));
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
  // TODO do animations
}

void LedSegment::callback(String receivedMessageTopic, String newValueString) {
  if(_ledCount == 0)
    return;

  int newValueInt = newValueString.toInt();
  byte newValue;
  if (newValueInt > 255)
    newValue = 255;
  else if (newValueInt < 0)
    newValue = 0;
  else 
    newValue = (byte)newValueInt;

  if (receivedMessageTopic.endsWith(_subtopicLevel)) {
    if (newValueInt > 100)
    newValue = 100;

    if (_level == newValue)
      return;

    if(EEPROM.read(_memoryAddressLevel) != newValue) {
      EEPROM.write(_memoryAddressLevel, newValue);
      EEPROM.commit();
    }
    _mqtt->publishState(_subtopicLevel, String(newValue));
    setLevel(newValue);
  } else if (receivedMessageTopic.endsWith(_subtopicAnimation)) {

    if (_animation == newValue)
      return;

    if(EEPROM.read(_memoryAddressAnimation) != newValue) {
      EEPROM.write(_memoryAddressAnimation, newValue);
      EEPROM.commit();
    }
    _mqtt->publishState(_subtopicAnimation, String(newValue));
    setAnimation(newValue);
  } else if (receivedMessageTopic.endsWith(_subtopicSpeed)) {

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
  for (int i = _ledOffset; i < _ledOffset + _ledCount; i++) {
    _leds->setLedWhite(i, _leds->linearPwm(newValue));
  }
}

void LedSegment::setAnimation(byte newValue) {
  _animation = newValue;
  // TODO set LEDs.
}

void LedSegment::setAnimationSpeed(byte newValue) {
  _speed = newValue;
  // TODO set LEDs.
}
