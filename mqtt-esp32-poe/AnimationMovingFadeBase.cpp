#include <EEPROM.h>
#include "AnimationMovingFadeBase.h"

void AnimationMovingFadeBase::afterBegin() {
  _animationStepCount = _ledOffset + _ledCount + MOVINGFADE_GRADIENT_STEP_COUNT;
}

void AnimationMovingFadeBase::afterStart() {
}

void AnimationMovingFadeBase::doAnimationStep(byte animationStep) {
  if (animationStep < 0 || animationStep >= _animationStepCount) {
    _isRunning = false;
    return;
  }

  int ledToSet, newLevel;
  bool fadeUp = _targetLevel >= _previousLevel;

  for (byte i = 0; i < MOVINGFADE_GRADIENT_STEP_COUNT; i++) {

    ledToSet = getLedToSet(i, animationStep);

    if (ledToSet >= _ledOffset + _ledCount || ledToSet < _ledOffset)
      continue;
    
    if (fadeUp)
      newLevel = min(movingFadeGradient[i], _targetLevel);
    else
      newLevel = max((byte)(100 - movingFadeGradient[i]), _targetLevel);

    if (fadeUp) {
      _leds->setLedWhiteMin(ledToSet, _leds->linearPwm(newLevel));
    } else {
      _leds->setLedWhiteMax(ledToSet, _leds->linearPwm(newLevel));
    }
  }

  _isRunning = animationStep < _animationStepCount;
}
