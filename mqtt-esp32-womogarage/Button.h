#include <Arduino.h>
#include "MqttPubSub.h"

#ifndef Button_h

#define BUTTON_STATE_INITIALIZED "initialized"
#define BUTTON_STATE_SINGLE "single"
#define BUTTON_STATE_DOUBLE "double"
#define BUTTON_STATE_TRIPLE "triple"
#define BUTTON_STATE_MANY "many"
#define BUTTON_STATE_LONG "long"
#define BUTTON_NUM_STATES 6

#define BUTTON_LONG_PRESS_MS 1000
#define BUTTON_RELEASE_MAX_DELAY 800
#define BUTTON_PRESS_MIN_MS 80

#define Button_h


class Button {
  private: 
  	byte _pin = 0;
    String _subtopic;
    String _subtopicState;
    String _subtopicStateIndex;
    int _memoryAddress;
    MqttPubSub* _mqtt;
    const char *_states[BUTTON_NUM_STATES] = {BUTTON_STATE_INITIALIZED, BUTTON_STATE_SINGLE, BUTTON_STATE_DOUBLE, BUTTON_STATE_TRIPLE, BUTTON_STATE_MANY, BUTTON_STATE_LONG};
    const byte _numStates = BUTTON_NUM_STATES;
    void publishinitialStates();
    unsigned long _lastPressStart = 0;
    unsigned long _lastReleaseStart = 0;
    byte _pressCount = 0;
    String getPossibleStatesJsonArray();
    bool pressed();
    void reset();

  public:
    void begin(byte pin, String subtopic, MqttPubSub* mqtt);
    void check();
};

#endif
