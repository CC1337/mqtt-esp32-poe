#include <Arduino.h>
#include "config.h"
#include "AddressableLeds.h"

#ifndef Animation_h
#define Animation_h


class Animation {
  protected:
    int _ledOffset;
    int _ledCount;
    AddressableLeds* _leds;
    byte _targetLevel;
    byte _previousLevel;
    bool _isRunning = false;

  public:
    virtual void afterBegin() = 0;
    virtual void afterStart() = 0;
    virtual String getName() = 0;
    virtual void doAnimationStep(byte animationStep) = 0;
    virtual void begin(int ledOffset, int ledCount, AddressableLeds* leds);
    virtual void start(byte targetLevel, byte previousLevel);
    bool isRunning();
};

#endif
