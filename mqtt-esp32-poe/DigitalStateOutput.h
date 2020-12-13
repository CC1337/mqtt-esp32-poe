#include <Arduino.h>
#include "MqttPubSub.h"

#ifndef DigitalStateOutput_h
#define DigitalStateOutput_h


class DigitalStateOutput {
  private: 
  	byte _pin = 0;
    bool _inverted;
    String _subtopic;
    int _memoryAddress;
    byte _publishedMsgCount = 0;
    MqttPubSub* _mqtt;
    void restoreFromEepromAndPublish();
    void setOutput(bool newState);

  public:
    void begin(byte pin, bool inverted, String subtopic, int memoryAddress, MqttPubSub* mqtt);
    void resubscribe();
    void callback(String receivedMessageTopic, String newState);
};

#endif
