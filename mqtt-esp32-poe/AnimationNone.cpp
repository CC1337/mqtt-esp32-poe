#include <EEPROM.h>
#include "AnimationNone.h"

void AnimationNone::afterBegin() {
}

void AnimationNone::afterStart() {
}

String AnimationNone::getName() {
  return "none";
}

void AnimationNone::doAnimationStep(byte animationStep) {
  for (int i = _ledOffset; i < _ledOffset + _ledCount; i++) {
    _leds->setLedWhite(i, _leds->linearPwm(_targetLevel));
  }
  _isRunning = false;
}
