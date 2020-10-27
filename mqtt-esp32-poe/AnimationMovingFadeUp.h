#include "AnimationMovingFadeBase.h"

#ifndef AnimationMovingFadeUp_h
#define AnimationMovingFadeUp_h

class AnimationMovingFadeUp : public AnimationMovingFadeBase {
  private:
    int getLedToSet(int gradientStep, int animationStep);
    
  public:
    String getName();
};

#endif
