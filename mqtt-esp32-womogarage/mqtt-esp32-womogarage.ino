#include <WiFiClient.h>
#include <PubSubClient.h>
#include "config.h"

#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_PHY_POWER 12

#include <ETH.h>


// Initializations of network clients
WiFiClient espClient;
PubSubClient mqttClient(espClient);

static bool eth_connected = false;
uint64_t chipid;
unsigned long lastHealthPing = millis();


// BEGIN TODO move to class..
void initStates() {
  for (byte i=0; i<StateTopicCount; i++)
    initState(String(StateTopics[i]));
}

void initState(String stateTopic) {
  String topicToSubscribe  = stateTopic + STATE_TOPIC_SETTER;
  subscribe(topicToSubscribe);
  publishInitialStateSetterTopics(topicToSubscribe);
}

void publishInitialStateSetterTopics(String setterTopic) {
  // TODO save/restore initial value from EEPROM
  //mqttClient.publish(stateTopic.c_str(), false);
  mqttClient.publish(setterTopic.c_str(), false);
}

void callbackState(String stateTopic, String newState) {
  bool targetState;
  if (newState == "true")
    targetState = true;
  if (newState == "false")
    targetState = false;
  else
    return;

  for (byte i=0; i<StateTopicCount; i++) {
    String currentTopic = String(StateTopics[i]);
    String currentTopicSetter = currentTopic + STATE_TOPIC_SETTER;
    if (stateTopic == currentTopicSetter) {
      // TODO actually set something
      Serial.print(currentTopicSetter);
      Serial.print(" = ");
      Serial.println(targetState);
      mqttClient.publish(currentTopic.c_str(), String(targetState).c_str());
    }
  }
}

// END TODO


void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname(MqttTopic);
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String payloadString;
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    payloadString.concat((char)payload[i]);
  }
  Serial.println();
  callbackState(String(topic), payloadString);
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(MqttClientName)) {
    //if (mqttClient.connect(MqttClientName, MqttUsername, MqttPassword) { // if credentials is nedded
      Serial.println("connected");
      publishStates();
     // mqttClient.subscribe("*");
     // mqttClient.subscribe(MqttTopic);
     // mqttClient.subscribe("ESP32-Wohnmobilgarage/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
 if(DEBUG)
    Serial.begin(115200);
  Serial.println(F("POWER ON."));

  mqttClient.setServer(MqttServerIp, MqttServerPort);
  mqttClient.setCallback(callback);

  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
  Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.

  WiFi.onEvent(WiFiEvent);

  ETH.begin();
  ETH.config(local_IP, gateway, subnet, gateway, gateway);
  
  pinMode(BWM_VORGARTEN_PIN, INPUT_PULLUP);
  pinMode(BWM_GARTEN_PIN, INPUT_PULLUP);
 
  Serial.println(F("Ready"));
}

void loop()
{
  // check if ethernet is connected
  if (eth_connected) {
    // now take care of MQTT client...
    if (!mqttClient.connected()) {
      reconnect();
    } else {
      mqttClient.loop();
    }
    healthPing(false);
  }
}

void publishStates() {
  String onlineText = String(MqttTopic);
  onlineText.concat(" online.");
  mqttPublish("status", "online", ip2Str(ETH.localIP()), onlineText);
  healthPing(true);
}

void mqttPublish(String subtopic, String action, String payload, String fullMessage) {
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
  subtopic = MqttTopic + subtopic;

  if(mqttClient.publish(subtopic.c_str(), message.c_str())) {
    Serial.println("Publish message success: " + message);
  } else {
    Serial.println("Could not send message: " + message);
  }
  delay(300);
}

void subscribe(String subtopic) {
  String topicToSubscribe = String(MqttTopic);
  topicToSubscribe.concat("/");
  topicToSubscribe.concat(subtopic);
  mqttClient.subscribe(topicToSubscribe.c_str());
}

void healthPing(bool force) {
  if (millis()-lastHealthPing < HealthPingDelayMs && !force)
    return;
  lastHealthPing = millis();
  Serial.println(F("Starting health ping... "));

  String payload = "isAlive_";
  payload.concat(String(random(65535)));
  mqttPublish("health", "health_ping", payload, "");

  Serial.println(F("end."));
}

String ip2Str(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++) {
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  }
  return s;
}
