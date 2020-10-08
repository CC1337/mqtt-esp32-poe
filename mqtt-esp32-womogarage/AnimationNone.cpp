#include <EEPROM.h>
#include "AnimationNone.h"

void AnimationNone::begin(int ledOffset, int ledCount, AddressableLeds* leds) {
  _ledOffset = ledOffset;
  _ledCount = ledCount;
  _leds = leds;
}

void AnimationNone::start(byte targetLevel) {
  _targetLevel = targetLevel;
  _isRunning = true;
}

String AnimationNone::getName() {
  return "none";
}

void AnimationNone::doAnimationStep() {
  for (int i = _ledOffset; i < _ledOffset + _ledCount; i++) {
    _leds->setLedWhite(i, _leds->linearPwm(_targetLevel));
  }
  _isRunning = false;
}

bool AnimationNone::isRunning() {
  return _isRunning;
}
