#include <Arduino.h>
#include "MqttPubSub.h"
#include "AddressableLeds.h"
#include "AnimationNone.h"
#include "AnimationFade.h"
#include "AnimationFlicker.h"
#include "AnimationMovingFade.h"

#ifndef LedSegment_h
#define LedSegment_h

/* Adding new animation:
 * - increase ANIMATIONS_COUNT below
 * - implement new .h/.cpp
 * - add include above for new animation .h
 * - adjust prepareAnimations() implementation in LedSegment.cpp file
 */
#define ANIMATIONS_COUNT 4
#define ANIMATION_NONE 0
#define ANIMATION_FADE 1
#define ANIMATION_FLICKER 2
#define ANIMATION_MOVINGFADE 3

class LedSegment {
  private: 
    int _ledOffset;
    int _ledCount = 0;
    String _subtopic;
    String _subtopicLevel;
    String _subtopicAnimation;
    String _subtopicAnimationIndex;
    String _subtopicSpeed;
    int _memoryAddress;
    int _memoryAddressLevel;
    int _memoryAddressAnimation;
    int _memoryAddressSpeed;
    byte _level = 0;
    byte _animation = 0;
    byte _speed = 100;
    byte _activeAnimation = 0;
    int _animationStep = -1;
    unsigned long _lastAnimationStepMicros = 0;
    MqttPubSub* _mqtt;
    AddressableLeds* _leds;
    Animation* _animations[ANIMATIONS_COUNT];
    const byte _numAnimations = ANIMATIONS_COUNT;
    void prepareAnimations();
    void restoreFromEepromAndPublish();
    String getPossibleAnimationsJsonArray();
    String number2AnimationName(byte number);
    byte animationName2Number(String animationName);
    void setLevel(byte newValue);
    void setAnimation(byte newValue);
    void setAnimationSpeed(byte newValue);
    byte strToPercentage(String newValueString);
    bool waitingForNextFrame();

  public:
    void begin(int firstLed, int ledOffset, String subtopic, int memoryAddress, AddressableLeds* leds, MqttPubSub* mqtt);
    void loop();
    void callback(String receivedMessageTopic, String newValue);
};

#endif
