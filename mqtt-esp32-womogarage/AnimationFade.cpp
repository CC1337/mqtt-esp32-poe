#include <EEPROM.h>
#include "AnimationFade.h"

void AnimationFade::afterBegin() {
}

void AnimationFade::afterStart() {
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

  _isRunning = animationStep != 100;
}
