#include <EEPROM.h>
#include "Animation.h"


void Animation::begin(int ledOffset, int ledCount, AddressableLeds* leds) {
  _ledOffset = ledOffset;
  _ledCount = ledCount;
  _leds = leds;
  afterBegin();
}

void Animation::start(byte targetLevel, byte previousLevel) {
  _targetLevel = targetLevel;
  _previousLevel = previousLevel;
  _isRunning = true;
  afterStart();
}

bool Animation::isRunning() {
  return _isRunning;
}
