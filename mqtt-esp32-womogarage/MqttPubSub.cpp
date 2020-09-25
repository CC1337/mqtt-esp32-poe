#include "MqttPubSub.h"

void MqttPubSub::begin(PubSubClient* mqttClient, String topic) {
  _mqttClient = mqttClient;
  _topic = topic;
  _initialized = true;
}

void MqttPubSub::subscribe(String subtopic) {
  if(!_initialized)
    return;
  
  String topicToSubscribe = _topic;
  topicToSubscribe.concat("/");
  topicToSubscribe.concat(subtopic);
  _mqttClient->subscribe(topicToSubscribe.c_str());
  Serial.println("Subscribed to: " + topicToSubscribe);
}

void MqttPubSub::publishState(String subtopic, String payload) {
  if(!_initialized)
    return;
  
  String topicToPublish = _topic;
  topicToPublish.concat("/");
  topicToPublish.concat(subtopic);
  if(_mqttClient->publish(topicToPublish.c_str(), payload.c_str())) {
    Serial.println("Publish message success: [" + topicToPublish + "] " + payload);
  } else {
    Serial.println("Could not send message: [" + topicToPublish + "] " + payload);
  }
}

void MqttPubSub::publishMessage(String subtopic, String action, String payload, String fullMessage) {
  if(!_initialized)
    return;
  
  String message = "{ \"action\": \"";
  message.concat(action.c_str());
  message.concat("\", \"payload\": \"");
  message.concat(payload.c_str());
  message.concat("\", \"fullMessage\": \"");
  message.concat(fullMessage.c_str());
  message.concat("\", \"randomNumber\": \"");
  message.concat(String(random(65535)).c_str());
  message.concat("\"}");
  subtopic = "/" + subtopic;
  subtopic = _topic + subtopic;

  if(_mqttClient->publish(subtopic.c_str(), message.c_str())) {
    Serial.println("Publish message success: " + message);
  } else {
    Serial.println("Could not send message: " + message);
  }
  delay(300);
}
