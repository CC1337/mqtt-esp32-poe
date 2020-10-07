#include <Arduino.h>
#include "MqttPubSub.h"
#include "AddressableLeds.h"

#ifndef AddressableLedPower_h
#define AddressableLedPower_h


class AddressableLedPower {
  private: 
    byte _pin;
    AddressableLeds* _leds;

  public:
    void begin(byte pin, AddressableLeds* leds);
    void loop();
    void on();
    void off();
};

#endif
