#include <EEPROM.h>
#include "DigitalStateOutput.h"

extern String bool2Str(bool buhl);

void DigitalStateOutput::begin(byte pin, bool inverted, String subtopic, int memoryAddress, MqttPubSub* mqtt) {
  _pin = pin;
  _inverted = inverted;
  _subtopic = subtopic;
  _mqtt = mqtt;
  _memoryAddress = memoryAddress;
  pinMode(_pin, OUTPUT);

  _mqtt->subscribe(_subtopic);
  restoreFromEepromAndPublish();
}

void DigitalStateOutput::restoreFromEepromAndPublish() {
  bool eepromState = (bool)EEPROM.read(_memoryAddress);
  setOutput(eepromState);
  _mqtt->publishState(_subtopic,  bool2Str(eepromState));
}

void DigitalStateOutput::setOutput(bool newState) {
  digitalWrite(_pin, _inverted ? !newState : newState);
}

void DigitalStateOutput::resubscribe() {
  _mqtt->resubscribe(_subtopic);
}

void DigitalStateOutput::callback(String receivedMessageTopic, String newState) {
  if(_pin == 0)
    return;

  if (receivedMessageTopic.endsWith(_subtopic)) {

    bool targetState;
    if (newState == "true" || newState == "1")
      targetState = true;
    else if (newState == "false" || newState == "0")
      targetState = false;
    else
      return;

    if (targetState == _inverted ? !digitalRead(_pin) : digitalRead(_pin)) {
      _publishedMsgCount = 0;
      return;
    }

    if (_publishedMsgCount > 1) {
      _publishedMsgCount--;
      Serial.print("Dropping possibly old message: ");
      Serial.print(_subtopic);
      Serial.print(F(" = "));
      Serial.println(targetState);
      return;
    }

    Serial.print(_subtopic);
    Serial.print(F(" = "));
    Serial.println(targetState);

    if(EEPROM.read(_memoryAddress) != targetState) {
      EEPROM.write(_memoryAddress, (byte)targetState);
      EEPROM.commit();
    }
    setOutput(targetState);
    _mqtt->publishState(_subtopic, bool2Str(targetState));
    _publishedMsgCount++;
  }
}
