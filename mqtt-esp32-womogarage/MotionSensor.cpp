#include "MotionSensor.h"

extern void publish(String subtopic, String payload);
extern String bool2Str(bool buhl);

void MotionSensor::begin(byte pin, String subtopic) {
  _pin = pin;
  _subtopic = subtopic;
  pinMode(_pin, INPUT);
}

void MotionSensor::check() {
  if(_pin == 0)
    return;
  
	bool val = digitalRead(_pin);

	if (val == _lastVal)
    return;
		
	Serial.print(F("Motion Sensor state changed on pin: "));
	Serial.print(_pin);
	Serial.print(F(" / Subtopic: "));
	Serial.println(_subtopic);

	publish(_subtopic, bool2Str(val));

  _lastVal = val;
}
