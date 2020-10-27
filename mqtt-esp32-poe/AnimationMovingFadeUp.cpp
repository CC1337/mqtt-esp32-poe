#include <EEPROM.h>
#include "AnimationMovingFadeUp.h"

void AnimationMovingFadeUp::afterBegin() {
  _animationStepCount = _ledOffset + _ledCount + MOVINGFADE_GRADIENT_STEP_COUNT;
}

void AnimationMovingFadeUp::afterStart() {
}

String AnimationMovingFadeUp::getName() {
  return "movingfadeup";
}

int AnimationMovingFadeUp::getLedToSet(int gradientStep, int animationStep) {
  return animationStep + _ledOffset - gradientStep;
}

void AnimationMovingFadeUp::doAnimationStep(byte animationStep) {
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
