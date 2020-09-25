#include <Arduino.h>
#include "MqttPubSub.h"

#ifndef MotionSensor_h
#define MotionSensor_h


class MotionSensor {
  private: 
  	byte _pin = 0;
    String _subtopic;
    MqttPubSub* _mqtt;
  	bool _lastVal = HIGH;

  public:
    void begin(byte pin, String subtopic, MqttPubSub* mqtt);
    void check();
};

#endif
