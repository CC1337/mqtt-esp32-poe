#include <EEPROM.h>
#include "InfoLed.h"

void InfoLed::begin(byte pin, String subtopic, int memoryAddress, MqttPubSub* mqtt) {
  _pin = pin;
  _subtopic = subtopic;
  _subtopicState = subtopic + "/state";
  _subtopicStateIndex = subtopic + "/states_possible";
  _mqtt = mqtt;
  _memoryAddress = memoryAddress;
  pinMode(_pin, OUTPUT);

  _mqtt->subscribe(_subtopicState);
  restoreFromEepromAndPublish();
}

void InfoLed::restoreFromEepromAndPublish() {
  byte eepromState = EEPROM.read(_memoryAddress);
  _mqtt->publishState(_subtopicState, number2State(eepromState));
  _mqtt->publishState(_subtopicStateIndex, getPossibleStatesJsonArray());
}

String InfoLed::getPossibleStatesJsonArray() {
  String statesPossible = String("[ ");
  for (int i=0; i<_numStates; i++) {
    statesPossible.concat("\"");
    statesPossible.concat(_states[i]);
    statesPossible.concat("\"");
    if (i < _numStates - 1)
      statesPossible.concat(", ");
  }
  statesPossible.concat(" ]");
  return statesPossible;
}

String InfoLed::number2State(byte number) {
  if (number < 0)
    return String(INFOLED_STATE_OFF);
  if (number >= _numStates)
    return String(INFOLED_STATE_ON);

  return String(_states[number]);
}

byte InfoLed::state2Number(String state) {
  for (int i=0; i<_numStates; i++) {
    if (String(_states[i]).equalsIgnoreCase(state))
      return i;
  }
  return 0;
}

void InfoLed::loop() {
  if (_blinkDelay == 0 || _blinkLength == 0)
    return;

  unsigned long elapsedTime = millis() - _lastBlinkStart;
  if (digitalRead(_pin) && elapsedTime > _blinkLength) {
    digitalWrite(_pin, LOW);
  } else if (!digitalRead(_pin) && elapsedTime > _blinkDelay) {
    digitalWrite(_pin, HIGH);
    _lastBlinkStart = millis();
  }
}

void InfoLed::setState(byte newState) {
  if (newState < 0 || newState >= _numStates)
    return;
  _state = newState;

  switch(_state) {
    case 2: // INFOLED_STATE_BLINK
      _blinkDelay = 1000;
      _blinkLength = 500;
      break;
    case 3: // INFOLED_STATE_BLINK_SLOW
      _blinkDelay = 2000;
      _blinkLength = 50;
      break;
    case 4: // INFOLED_STATE_BLINK_FAST
      _blinkDelay = 250;
      _blinkLength = 100;
      break;
    default:
      _blinkDelay = 0;
      _blinkLength = 0;
      break;
  }
  
  digitalWrite(_pin, newState != 0);
  _lastBlinkStart = millis();
}

void InfoLed::callback(String receivedMessageTopic, String newStateStr) {
  if(_pin == 0)
    return;

  if (receivedMessageTopic.endsWith(_subtopicState)) {
    byte targetStateNum = state2Number(newStateStr);

    if (targetStateNum == _state)
      return;

    Serial.print(_subtopic);
    Serial.print(F(" = "));
    Serial.println(targetStateNum);

    if(EEPROM.read(_memoryAddress) != targetStateNum) {
      EEPROM.write(_memoryAddress, (byte)targetStateNum);
      EEPROM.commit();
    }
    setState(targetStateNum);
    _mqtt->publishState(_subtopicState, number2State(targetStateNum));
  }
}
