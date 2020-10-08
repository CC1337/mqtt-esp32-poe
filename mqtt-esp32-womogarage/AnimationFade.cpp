#include <EEPROM.h>
#include "AnimationFade.h"

void AnimationFade::begin(int ledOffset, int ledCount, AddressableLeds* leds) {
  _ledOffset = ledOffset;
  _ledCount = ledCount;
  _leds = leds;
}

void AnimationFade::start(byte targetLevel) {
  _targetLevel = targetLevel;
  _isRunning = true;
}

String AnimationFade::getName() {
  return "fade";
}

void AnimationFade::doAnimationStep(byte animationStep) {
  if (animationStep < 0 || animationStep > 100) {
    _isRunning = false;
    return;
  }

  for (int i = _ledOffset; i < _ledCount + _ledOffset; i++) {
    _leds->ledFadeStepToPercentage(i, _targetLevel, animationStep);
  }

  _isRunning = animationStep == 100;
}

bool AnimationFade::isRunning() {
  return _isRunning;
}
