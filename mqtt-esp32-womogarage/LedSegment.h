#include <Arduino.h>
#include "MqttPubSub.h"
#include "AddressableLeds.h"
#include "AnimationNone.h"
#include "AnimationFade.h"

#ifndef LedSegment_h
#define LedSegment_h

#define ANIMATIONS_COUNT 2
#define ANIMATION_NONE 0
#define ANIMATION_FADE 1

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
    byte _level;
    byte _animation;
    byte _speed;
    byte _activeAnimation = 0;
    int _animationStep = -1;
    MqttPubSub* _mqtt;
    AddressableLeds* _leds;
    AnimationNone _animationNone;
    AnimationFade _animationFade;
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
