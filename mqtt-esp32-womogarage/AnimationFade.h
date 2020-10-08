#include <Arduino.h>
#include "config.h"
#include "AddressableLeds.h"

#ifndef AnimationFade_h
#define AnimationFade_h


class AnimationFade {
  private:
    int _ledOffset;
    int _ledCount;
    AddressableLeds* _leds;
    byte _targetLevel;
    bool _isRunning = false;

  public:
    void begin(int ledOffset, int ledCount, AddressableLeds* leds);
    void start(byte targetLevel);
    String getName();
    void doAnimationStep(byte animationStep);
    bool isRunning();
};

#endif
