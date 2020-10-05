#include "AddressableLeds.h"

void AddressableLeds::begin(int pin, int ledCount) {
  _pin = pin;
  _ledCount = ledCount;
}

void AddressableLeds::show() {
  //TODO FastLED.show(); if power on, animation step, power management
}
