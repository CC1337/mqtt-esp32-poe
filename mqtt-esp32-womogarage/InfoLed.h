#include <Arduino.h>
#include "MqttPubSub.h"

#ifndef InfoLed_h

#define INFOLED_STATE_OFF "off"
#define INFOLED_STATE_ON "on"
#define INFOLED_STATE_BLINK "blink"
#define INFOLED_STATE_BLINK_SLOW "blink-slow"
#define INFOLED_STATE_BLINK_FAST "blink-fast"
#define INFOLED_NUM_STATES 5

#define InfoLed_h


class InfoLed {
  private: 
  	byte _pin = 0;
    String _subtopic;
    String _subtopicState;
    String _subtopicStateIndex;
    int _memoryAddress;
    MqttPubSub* _mqtt;
    const char *_states[INFOLED_NUM_STATES] = {INFOLED_STATE_OFF, INFOLED_STATE_ON, INFOLED_STATE_BLINK, INFOLED_STATE_BLINK_SLOW, INFOLED_STATE_BLINK_FAST};
    const byte _numStates = INFOLED_NUM_STATES;
    byte _state = 0;    void restoreFromEepromAndPublish();
    unsigned long _lastBlinkStart = 0;
    int _blinkDelay = 0;
    int _blinkLength = 0;
    String getPossibleStatesJsonArray();
    String number2State(byte number);
    byte state2Number(String state);
    void setState(byte newState);

  public:
    void begin(byte pin, String subtopic, int memoryAddress, MqttPubSub* mqtt);
    void callback(String receivedMessageTopic, String newState);
    void check();
};

#endif
