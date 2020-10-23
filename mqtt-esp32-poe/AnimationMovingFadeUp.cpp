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

  for (byte i = 0; i < MOVINGFADE_GRADIENT_STEP_COUNT; i++) {

    ledToSet = getLedToSet(i, animationStep);

    if (ledToSet >= _ledOffset + _ledCount || ledToSet < _ledOffset)
      continue;
    
    if (_targetLevel >= _previousLevel)
      newLevel = movingFadeGradient[i];
    else
      newLevel = 100 - movingFadeGradient[i];

    _leds->ledFadeStepToPercentage(ledToSet, newLevel, 100);
  }

  _isRunning = animationStep < _animationStepCount;
}
