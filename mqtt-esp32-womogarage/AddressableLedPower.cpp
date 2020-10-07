#include <EEPROM.h>
#include "AddressableLedPower.h"

void AddressableLedPower::begin(byte pin, AddressableLeds* leds) {
  _pin = pin;
  _leds = leds;
  pinMode(_pin, OUTPUT);
}

void AddressableLedPower::loop() {
  // TODO control power pin
}

void AddressableLedPower::on() {
  Serial.println(F("LED Power ON"));
  digitalWrite(_pin, ADDRESSABLE_LED_POWER_PIN_INVERT ? LOW : HIGH);
}

void AddressableLedPower::off() {
  Serial.println(F("LED Power OFF"));
  digitalWrite(_pin, ADDRESSABLE_LED_POWER_PIN_INVERT ? HIGH : LOW);
}

bool AddressableLedPower::isOn() {
  return ADDRESSABLE_LED_POWER_PIN_INVERT ? !digitalRead(_pin) : digitalRead(_pin);
}
