#include <Arduino.h>
#include "MqttPubSub.h"
#include "AddressableLeds.h"
#include "AnimationNone.h"
#include "AnimationFade.h"
#include "AnimationFlicker.h"

#ifndef LedSegment_h
#define LedSegment_h

#define ANIMATIONS_COUNT 3
#define ANIMATION_NONE 0
#define ANIMATION_FADE 1
#define ANIMATION_FLICKER 2

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
    long _lastAnimationStepMillis = 0;
    MqttPubSub* _mqtt;
    AddressableLeds* _leds;
    AnimationNone _animationNone;
    AnimationFade _animationFade;
    AnimationFlicker _animationFlicker;
    String _animationNames[ANIMATIONS_COUNT];
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

  public:
    void begin(int firstLed, int ledOffset, String subtopic, int memoryAddress, AddressableLeds* leds, MqttPubSub* mqtt);
    void loop();
    void callback(String receivedMessageTopic, String newValue);
};

#endif
