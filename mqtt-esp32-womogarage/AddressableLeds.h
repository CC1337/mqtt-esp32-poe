#include <Arduino.h>

#ifndef AddressableLeds_h
#define AddressableLeds_h

class AddressableLeds {
  private: 
    int _ledCount = 0;
    byte _pin = 0;

  public:
    void begin(int pin, int ledCount);
    void show();
};

#endif
