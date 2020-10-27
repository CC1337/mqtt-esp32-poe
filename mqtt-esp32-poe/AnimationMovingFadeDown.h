#include "AnimationMovingFadeBase.h"

#ifndef AnimationMovingFadeDown_h
#define AnimationMovingFadeDown_h

class AnimationMovingFadeDown : public AnimationMovingFadeBase {
  private:
    int getLedToSet(int gradientStep, int animationStep);
    
  public:
    String getName();
};

#endif
