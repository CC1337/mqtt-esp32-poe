#include "Button.h"
#include "config.h"

void Button::begin(byte pin, String subtopic, MqttPubSub* mqtt) {
  _pin = pin;
  _subtopic = subtopic;
  _subtopicState = subtopic + "/state";
  _subtopicStateIndex = subtopic + "/states_possible";
  _mqtt = mqtt;
  pinMode(_pin, INPUT_PULLUP);

  publishinitialStates();
}

void Button::publishinitialStates() {
  _mqtt->publishState(_subtopicState, BUTTON_STATE_NONE);
  _mqtt->publishState(_subtopicStateIndex, getPossibleStatesJsonArray());
}

String Button::getPossibleStatesJsonArray() {
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

void Button::loop() {
  if (pressed()) {
    _lastReleaseStart = 0;
    if (_lastPressStart == 0) {
      _lastPressStart = millis();
    } else if (millis() - _lastPressStart >= BUTTON_LONG_PRESS_MS && _pressCount == 0) {
      _mqtt->publishState(_subtopicState, BUTTON_STATE_LONG);
      _pressCount = 1;
    }
  } else {
    if (_lastPressStart > 0) {
      if (millis() - _lastPressStart < BUTTON_PRESS_MIN_MS) { // entprell
        _lastPressStart = 0;
      } else if (millis() - _lastPressStart < BUTTON_LONG_PRESS_MS) {
        _lastPressStart = 0;
        _lastReleaseStart = millis();
        _pressCount++;
      } else {
        reset();
      }
    } else if (_pressCount > 0 && millis() - _lastReleaseStart > BUTTON_RELEASE_MAX_DELAY){
      if (_pressCount == 1)
        _mqtt->publishState(_subtopicState, BUTTON_STATE_SINGLE);
      else if (_pressCount == 2)
        _mqtt->publishState(_subtopicState, BUTTON_STATE_DOUBLE);
      else if (_pressCount == 3)
        _mqtt->publishState(_subtopicState, BUTTON_STATE_TRIPLE);
      else if (_pressCount > 3)
        _mqtt->publishState(_subtopicState, BUTTON_STATE_MANY);
      reset();
    }
  }
}

bool Button::pressed() {
  return digitalRead(_pin) == LOW;
}

void Button::reset() {
  _lastPressStart = 0;
  _lastReleaseStart = 0;
  _pressCount = 0;
}
