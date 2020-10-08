#include "AddressableLeds.h"

void AddressableLeds::begin(int pin, int ledCount, AddressableLedPower* ledPower) {
  _pin = pin;
  _ledCount = ledCount;
  _ledPower = ledPower;
  ledArray = new CRGB[_ledCount];
}

void AddressableLeds::show() {
  _ledPower->updatePowerstate(isAnyLedOn());
  if (_ledPower->isOn()) {
    FastLED.show();
  }

  // if power on, animation step, power management
}

byte AddressableLeds::getPin() {
  return _pin;
}

int AddressableLeds::getCount() {
  return _ledCount;
}

byte AddressableLeds::linearPwm(byte percentage) {
  return _linear_pwm[percentage];
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

void AddressableLeds::ledFadeStepToPercentage(int index, byte targetLevel, byte fadePercentage) {
  byte newBrightness;
  byte currentBrightness = getLedWhite(index);
  byte targetBrightness = linearPwm(targetLevel);

  if (currentBrightness < targetBrightness) {
    // FADE UP
    newBrightness = linearPwm(fadePercentage);
    if (newBrightness > targetBrightness) {
      // END
      setLedWhite(index, targetBrightness);
    }
    else {
      setLedWhiteMin(index, newBrightness);
    }
  }
  else {
    // FADE DOWN
    newBrightness = linearPwm(100 - fadePercentage);
    if (newBrightness < targetBrightness) {
      // END
      setLedWhite(index, targetBrightness);
    }
    else {
      setLedWhiteMax(index, newBrightness);
    }
  }
}

bool AddressableLeds::isAnyLedOn() {
  for (int i=0; i<_ledCount; i++) {
    CRGB crgb_val = getLedCRGB(i);
    if (crgb_val.r + crgb_val.g + crgb_val.b != 0) {
      return true;
    }
  }
  return false;
}
