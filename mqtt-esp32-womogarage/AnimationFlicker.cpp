#include <EEPROM.h>
#include "AnimationFlicker.h"

void AnimationFlicker::begin(int ledOffset, int ledCount, AddressableLeds* leds) {
  _ledOffset = ledOffset;
  _ledCount = ledCount;
  _leds = leds;
  _randomIndex = new int[_ledCount];
  for (int i = 0; i < _ledCount; i++)
    _randomIndex[i] = _ledOffset + i;
}

void AnimationFlicker::start(byte targetLevel, byte previousLevel) {
  _targetLevel = targetLevel;
  _previousLevel = previousLevel;
  _step1Level = (2 * previousLevel + targetLevel) / 3;
  _step2Level = (previousLevel + 2 * targetLevel) / 3;
  for (int i = 0; i < _ledCount; i++)
    swapRandomIndex(i, i + (rand() % _ledCount-1-i));
  for (int i = 0; i < _ledCount; i++) {
    Serial.print(_randomIndex[i]);Serial.print(", ");
  }
  Serial.println();
  _isRunning = true;
}

void AnimationFlicker::swapRandomIndex(int a, int b) {
  int valA = _randomIndex[a];
  _randomIndex[a] = _randomIndex[b];
  _randomIndex[b] = valA;
}

String AnimationFlicker::getName() {
  return "flicker";
}

void AnimationFlicker::doAnimationStep(byte animationStep) {
  if (animationStep < 0 || animationStep > 100) {
    _isRunning = false;
    return;
  }

  int ledsPerAnimationStep = max(1, (int)ceil((float)_ledCount / 100.0));
  int startIndex = ledsPerAnimationStep * animationStep;
  int endIndex = min(startIndex + ledsPerAnimationStep, _ledCount);
  Serial.println("ledper " + String(ledsPerAnimationStep)); 
  Serial.println("start " + String(startIndex));
  Serial.println("end " + String(endIndex));

  for (int i = startIndex; i < endIndex; i++) {
    _leds->setLedWhite(_randomIndex[i], _targetLevel);
    Serial.println(String(millis()) + "set " + String(i) + "=" + String(_targetLevel));
  }

  // TODO: multiple stages

  _isRunning = animationStep != 100;
}

bool AnimationFlicker::isRunning() {
  return _isRunning;
}
