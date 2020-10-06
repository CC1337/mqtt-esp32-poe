#include "AddressableLeds.h"

void AddressableLeds::begin(int pin, int ledCount) {
  _pin = pin;
  _ledCount = ledCount;
  ledArray = new CRGB[_ledCount];
}

void AddressableLeds::show() {
  FastLED.show();
  // if power on, animation step, power management
}

byte AddressableLeds::getPin() {
  return _pin;
}

int AddressableLeds::getCount() {
  return _ledCount;
}

CRGB AddressableLeds::getLedCRGB(int index) {
  return ledArray[index];
}

int AddressableLeds::getLedWhite(int index) {
  CRGB crgb_val = getLedCRGB(index);
  return (crgb_val.r + crgb_val.g + crgb_val.b) / 3; // semi correct ;)
}

void AddressableLeds::setLedCRGB(int index, CRGB val) {
  ledArray[index] = val;
}

void AddressableLeds::setLedWhite(int index, byte val) {
  CRGB crgb_val = CRGB(val, val, val);
  setLedCRGB(index, crgb_val);
}

void AddressableLeds::setLedWhiteMin(int index, byte val) {
  CRGB crgb_val = CRGB(val, val, val);
  setLedCRGB(index, getLedCRGB(index) |= crgb_val);
}

void AddressableLeds::setLedWhiteMax(int index, byte val) {
  CRGB crgb_val = CRGB(val, val, val);
  setLedCRGB(index, getLedCRGB(index) &= crgb_val);
}

byte AddressableLeds::linearPwm(byte percentage) {
  return _linear_pwm[percentage];
}
