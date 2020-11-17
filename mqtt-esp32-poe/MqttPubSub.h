#include <PubSubClient.h>

#ifndef MqttPubSub_h
#define MqttPubSub_h


class MqttPubSub {
  private: 
  	PubSubClient* _mqttClient;
    String _topic;
  	bool _initialized = false;

  public:
    void begin(PubSubClient* mqttClient, String topic);
    void subscribe(String subtopic);
    void resubscribe(String subtopic);
    void publishState(String subtopic, String payload);
    void publishMessage(String subtopic, String action, String payload, String fullMessage);
};

#endif
