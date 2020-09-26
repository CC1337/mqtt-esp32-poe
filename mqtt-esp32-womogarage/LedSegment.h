#include <Arduino.h>
#include "MqttPubSub.h"

#ifndef LedSegment_h
#define LedSegment_h


class LedSegment {
  private: 
    int _ledOffset;
    int _ledCount = 0;
    String _subtopic;
    String _subtopicLevel;
    String _subtopicAnimation;
    String _subtopicSpeed;
    int _memoryAddress;
    int _memoryAddressLevel;
    int _memoryAddressAnimation;
    int _memoryAddressSpeed;
    byte _level;
    byte _animation;
    byte _speed;
    MqttPubSub* _mqtt;
    void restoreFromEepromAndPublish();
    void setLevel(byte newValue);
    void setAnimation(byte newValue);
    void setSpeed(byte newValue);

  public:
    void begin(int firstLed, int ledOffset, String subtopic, int memoryAddress, MqttPubSub* mqtt);
    void callback(String receivedMessageTopic, String newValue);
};

#endif
