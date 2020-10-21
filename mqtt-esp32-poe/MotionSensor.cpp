#include "MotionSensor.h"

extern String bool2Str(bool buhl);

void MotionSensor::begin(byte pin, String subtopic, MqttPubSub* mqtt) {
  _pin = pin;
  _subtopic = subtopic;
  _mqtt = mqtt;
  pinMode(_pin, INPUT);
}

void MotionSensor::loop() {
  if(_pin == 0)
    return;
  
	bool val = digitalRead(_pin);

	if (val == _lastVal)
    return;
		
	Serial.print(F("Motion Sensor state changed on pin: "));
	Serial.print(_pin);
	Serial.print(F(" / Subtopic: "));
	Serial.println(_subtopic);

	_mqtt->publishState(_subtopic, bool2Str(val));

  _lastVal = val;
}
