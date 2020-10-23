#include "AnimationMovingFadeUp.h"

#ifndef AnimationMovingFadeDown_h
#define AnimationMovingFadeDown_h

class AnimationMovingFadeDown : public AnimationMovingFadeUp {
  private:
    int getLedToSet(int gradientStep, int animationStep);
    
  public:
    String getName();
};

#endif
