#include <EEPROM.h>
#include "AnimationFlicker.h"

void AnimationFlicker::begin(int ledOffset, int ledCount, AddressableLeds* leds) {
  _ledOffset = ledOffset;
  _ledCount = ledCount;
  _leds = leds;
  _randomIndex = new int[_ledCount];
  _randomIndex2 = new int[_ledCount];
  _randomIndex3 = new int[_ledCount];
  for (int i = 0; i < _ledCount; i++) {
    _randomIndex[i] = _ledOffset + i;
    _randomIndex2[i] = _ledOffset + i;
    _randomIndex3[i] = _ledOffset + i;
  }

}

void AnimationFlicker::start(byte targetLevel, byte previousLevel) {
  _targetLevel = targetLevel;
  _previousLevel = previousLevel;
  _step1Level = (2 * previousLevel + targetLevel) / 3;
  _step2Level = (previousLevel + 2 * targetLevel) / 3;
  for (int i = 0; i < _ledCount; i++) {
    swapRandomIndex(_randomIndex, i, i + (rand() % _ledCount-i));
    swapRandomIndex(_randomIndex2, i, i + (rand() % _ledCount-i));
    swapRandomIndex(_randomIndex3, i, i + (rand() % _ledCount-i));
  }
  _isRunning = true;
}

void AnimationFlicker::swapRandomIndex(int* arr, int a, int b) {
  int valA = arr[a];
  arr[a] = arr[b];
  arr[b] = valA;
}

String AnimationFlicker::getName() {
  return "flicker";
}

void AnimationFlicker::doAnimationStep(byte animationStep) {
  if (animationStep < 0 || animationStep > 100) {
    _isRunning = false;
    return;
  }

  byte newLevel = 0;
  byte targetPwm = _leds->linearPwm(_targetLevel);
  int ledsPerAnimationStep = max(1, (int)ceil((float)_ledCount / 100.0));
  int startIndex = ledsPerAnimationStep * animationStep;
  int endIndex = min(startIndex + ledsPerAnimationStep, _ledCount);

  for (int i = startIndex; i < endIndex; i++) {
    newLevel = (9*_leds->getLedWhite(_randomIndex[i]) + targetPwm)/10;
    if (_leds->getLedWhite(_randomIndex[i]) != targetPwm)
      _leds->setLedWhite(_randomIndex[i], newLevel);
    //Serial.println(String(millis()) + "1set " + String(i) + "=" + String(newLevel));
  }

  byte stageStartAnimationStep = min((_ledCount / 2) * ledsPerAnimationStep, 30);
  int animationStepStage = animationStep - stageStartAnimationStep;

  // Stage 2
  if (animationStepStage >= 0) {
    ledsPerAnimationStep = max(1, (int)ceil((float)_ledCount / (100.0 - (float)stageStartAnimationStep)));
    startIndex = ledsPerAnimationStep * animationStepStage;
    endIndex = min(startIndex + ledsPerAnimationStep, _ledCount);

    for (int i = startIndex; i < endIndex; i++) {
      if (_leds->getLedWhite(_randomIndex2[i]) == targetPwm)
        continue;
      newLevel = _leds->linearPwm((6*_leds->getLedWhite(_randomIndex2[i]) + targetPwm)/7);
        _leds->setLedWhite(_randomIndex2[i], newLevel);
      //Serial.println(String(millis()) + "  2set " + String(i) + "=" + String(newLevel));
    }
  }

  stageStartAnimationStep = min(_ledCount * ledsPerAnimationStep, 60);
  animationStepStage = animationStep - stageStartAnimationStep;

  // Stage 3
  if (animationStepStage >= 0) {
    ledsPerAnimationStep = max(1, (int)ceil((float)_ledCount / (100.0 - (float)stageStartAnimationStep)));
    startIndex = ledsPerAnimationStep * animationStepStage;
    endIndex = min(startIndex + ledsPerAnimationStep, _ledCount);

    for (int i = startIndex; i < endIndex; i++) {
      _leds->setLedWhite(_randomIndex3[i], targetPwm);
      //Serial.println(String(millis()) + "    3set " + String(i) + "=" + String(_targetLevel));
    }
  }

  _isRunning = animationStep != 100;
}

bool AnimationFlicker::isRunning() {
  return _isRunning;
}
