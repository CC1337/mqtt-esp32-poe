#include <Arduino.h>
#include "config.h"
#include "AddressableLeds.h"

#ifndef AnimationNone_h
#define AnimationNone_h


class AnimationNone {
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
    void doAnimationStep();
    bool isRunning();
};

#endif
