#include "Animation.h"

#ifndef AnimationMovingFadeUp_h
#define AnimationMovingFadeUp_h

#define MOVINGFADE_GRADIENT_STEP_COUNT 70 // Movingfade gradient width LED count: higher = softer fade. Correlates with array movingFadeGradient below.

class AnimationMovingFadeUp : public Animation {
  private:
    int _animationStepCount;
    int getLedToSet(int gradientStep, int animationStep);
    const byte movingFadeGradient[MOVINGFADE_GRADIENT_STEP_COUNT] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 31, 32, 34, 35, 37, 38, 40, 41, 42, 44, 45, 47, 48, 50, 51, 52, 54, 55, 57, 58, 60, 61, 62, 64, 65, 67, 68, 70, 71, 72, 74, 75, 77, 78, 80, 81, 82, 84, 85, 87, 88, 90, 91, 92, 94, 95, 97, 98, 100 };

  public:
    void afterBegin();
    void afterStart();
    String getName();
    void doAnimationStep(byte animationStep);
};

#endif
