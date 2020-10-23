#include <EEPROM.h>
#include "AnimationMovingFadeDown.h"

String AnimationMovingFadeDown::getName() {
  return "movingfadedown";
}

int AnimationMovingFadeDown::getLedToSet(int gradientStep, int animationStep) {
  return -animationStep + _ledOffset + _ledCount + gradientStep;
}
