#include "Animation.h"

#ifndef AnimationFade_h
#define AnimationFade_h


class AnimationFade : public Animation {
  private:

  public:
    void afterBegin();
    void afterStart();
    String getName();
    void doAnimationStep(byte animationStep);
};

#endif
