#include <Arduino.h>
#include "config.h"

#ifndef AddressableLedPower_h
#define AddressableLedPower_h


class AddressableLedPower {
  private: 
    byte _pin;
    long _ledsLastOnMillis = 0;

  public:
    void begin(byte pin);
    void updatePowerstate(bool isAnyLedOn);
    void on();
    void off();
    bool isOn();
};

#endif
