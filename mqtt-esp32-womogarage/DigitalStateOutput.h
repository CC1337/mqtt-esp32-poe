#include <Arduino.h>
#include "MqttPubSub.h"

#ifndef DigitalStateOutput_h
#define DigitalStateOutput_h


class DigitalStateOutput {
  private: 
  	byte _pin = 0;
    String _subtopic;
    String _subtopicSet;
    String _subtopicGet;
    int _memoryAddress;
    MqttPubSub* _mqtt;
  	bool _lastVal = HIGH;
    void restoreFromEepromAndPublish();

  public:
    void begin(byte pin, String subtopic, int memoryAddress, MqttPubSub* mqtt);
    void callback(String receivedMessageTopic, String newState);
};

#endif
