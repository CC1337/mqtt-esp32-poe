#include <EEPROM.h>
#include "LedSegment.h"

void LedSegment::begin(int ledOffset, int ledCount, String subtopic, int memoryAddress, MqttPubSub* mqtt) {
  Serial.print("Init LedSegment - ledOffset: " + ledOffset);
  Serial.print(" | ledCount: " + ledCount);
  Serial.print(" | subtopic: " + subtopic);
  Serial.println(" | memoryAddress: " + memoryAddress);

  _ledOffset = ledOffset;
  _ledCount = ledCount;
  _subtopic = subtopic;
  _subtopicLevel = subtopic + "/level";
  _subtopicAnimation = subtopic + "/animation";
  _subtopicSpeed = subtopic + "/speed";
  _mqtt = mqtt;
  _memoryAddress = memoryAddress;
  _memoryAddressLevel = memoryAddress;
  _memoryAddressAnimation = memoryAddress + 1;
  _memoryAddressSpeed = memoryAddress + 2;

  _mqtt->subscribe(_subtopicLevel);
  _mqtt->subscribe(_subtopicAnimation);
  _mqtt->subscribe(_subtopicSpeed);
  restoreFromEepromAndPublish();
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
  _mqtt->publishState(_subtopicSpeed, String(eepromSpeed));
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
  // TODO set LEDs.
}

void LedSegment::setAnimation(byte newValue) {
  _animation = newValue;
  // TODO set LEDs.
}

void LedSegment::setAnimationSpeed(byte newValue) {
  _speed = newValue;
  // TODO set LEDs.
}
