#include <EEPROM.h>
#include "AnimationMovingFadeUp.h"

String AnimationMovingFadeUp::getName() {
  return "movingfadeup";
}

int AnimationMovingFadeUp::getLedToSet(int gradientStep, int animationStep) {
  return animationStep + _ledOffset - gradientStep;
}
