#include <Arduino.h>

#ifndef MotionSensor_h
#define MotionSensor_h


class MotionSensor {
  private: 
  	byte _pin = 0;
    String _subtopic;
  	bool _lastVal = HIGH;

  public:
    void begin(byte pin, String subtopic);
    void check();
};

#endif
