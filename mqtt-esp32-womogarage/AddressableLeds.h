#include <Arduino.h>
#include <FastLED.h>
#include "config.h"

#ifndef AddressableLeds_h
#define AddressableLeds_h

class AddressableLeds {
  private: 
    int _ledCount = 0;
    byte _pin = 0;
    const byte _linear_pwm[101] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 77, 80, 83, 86, 89, 92, 95, 98, 101, 104, 107, 110, 113, 116, 119, 122, 125, 129, 133, 137, 141, 145, 149, 153, 157, 161, 165, 169, 173, 178, 183, 188, 193, 198, 203, 208, 213, 218, 223, 228, 233, 238, 243, 248, 253, 255};

    
  public:
    CRGB* ledArray;
    void begin(int pin, int ledCount);
    void show();
    byte getPin();
    int getCount();
    byte linearPwm(byte percentage);
    CRGB getLedCRGB(int index);
    int getLedWhite(int index);
    void setLedCRGB(int index, CRGB val);
    void setLedWhite(int index, byte val);
    void setLedWhiteMin(int index, byte val);
    void setLedWhiteMax(int index, byte val);
    bool isAnyLedOn();

};

#endif
