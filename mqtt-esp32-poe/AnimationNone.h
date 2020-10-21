#include "Animation.h"

#ifndef AnimationNone_h
#define AnimationNone_h


class AnimationNone : public Animation {
  private:

  public:
    void afterBegin();
    void afterStart();
    String getName();
    void doAnimationStep(byte animationStep);
};

#endif
