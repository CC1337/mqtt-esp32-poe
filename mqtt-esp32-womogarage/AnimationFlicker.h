#include <Arduino.h>
#include "config.h"
#include "AddressableLeds.h"

#ifndef AnimationFlicker_h
#define AnimationFlicker_h


class AnimationFlicker {
  private:
    int _ledOffset;
    int _ledCount;
    AddressableLeds* _leds;
    byte _targetLevel;
    byte _previousLevel;
    byte _step1Level;
    byte _step2Level;
    bool _isRunning = false;
    int *_randomIndex;
    int *_randomIndex2;
    int *_randomIndex3;
    void swapRandomIndex(int* arr, int a, int b);


  public:
    void begin(int ledOffset, int ledCount, AddressableLeds* leds);
    void start(byte targetLevel, byte previousLevel);
    String getName();
    void doAnimationStep(byte animationStep);
    bool isRunning();
};

#endif
