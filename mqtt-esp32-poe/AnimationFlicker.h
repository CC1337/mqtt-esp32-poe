#include "Animation.h"

#ifndef AnimationFlicker_h
#define AnimationFlicker_h


class AnimationFlicker : public Animation {
  private:
    byte _step1Level;
    byte _step2Level;
    int *_randomIndex;
    int *_randomIndex2;
    int *_randomIndex3;
    void swapRandomIndex(int* arr, int a, int b);

  public:
    void afterBegin();
    void afterStart();
    String getName();
    void doAnimationStep(byte animationStep);
};

#endif
