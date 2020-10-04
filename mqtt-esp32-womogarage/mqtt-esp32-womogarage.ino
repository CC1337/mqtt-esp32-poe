#include <EEPROM.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "config.h"
#include "MqttPubSub.h"
#include "MotionSensor.h"
#include "Button.h"
#include "DigitalStateOutput.h"
#include "InfoLed.h"
#include "LedSegment.h"

#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_PHY_POWER 12

#include <ETH.h>


// Initializations of network clients
WiFiClient espClient;
PubSubClient mqttClient(espClient);
MqttPubSub mqtt;

static bool eth_connected = false;
uint64_t chipid;
unsigned long lastHealthPing = millis();
boolean mqttHasBeenInitializedBefore = false;


String bool2Str(bool buhl) {
  return buhl ? "true" : "false";
}

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

  callbackDigitalStateOutputs(String(topic), payloadString);
  callbackInfoLeds(String(topic), payloadString);
  callbackLedSegments(String(topic), payloadString);
}


// MOTION SENSORS

MotionSensor motionSensors[MotionSensorCount];

void initMotionSensors() {
  for (byte i=0; i<MotionSensorCount; i++)
    motionSensors[i].begin(MotionSensorPins[i], MotionSensorTopics[i], &mqtt);
}

void checkMotionSensors() {
  for (byte i=0; i<MotionSensorCount; i++)
    motionSensors[i].check();
}


// BUTTONS

Button buttons[ButtonCount];

void initButtons() {
  for (byte i=0; i<ButtonCount; i++)
    buttons[i].begin(ButtonPins[i], ButtonTopics[i], &mqtt);
}

void checkButtons() {
  for (byte i=0; i<ButtonCount; i++)
    buttons[i].check();
}


// DIGITAL STATE OUTPUTS

DigitalStateOutput digitalStateOutputs[DigitalStateOutputCount];

void initDigitalStateOutputs() {
  for (byte i=0; i<DigitalStateOutputCount; i++)
    digitalStateOutputs[i].begin(DigitalStateOutputPins[i], DigitalStateOutputInverted[i], DigitalStateOutputTopics[i], DigitalStateOutputMemoryAddresses[i], &mqtt);
}

void callbackDigitalStateOutputs(String messageTopic, String newState) {
  for (byte i=0; i<DigitalStateOutputCount; i++) {
    digitalStateOutputs[i].callback(messageTopic, newState);
  }
}


// INFO LEDS

InfoLed infoLeds[InfoLedCount];

void initInfoLeds() {
  for (byte i=0; i<InfoLedCount; i++)
    infoLeds[i].begin(InfoLedPins[i], InfoLedTopics[i], InfoLedMemoryAddresses[i], &mqtt);
}

void callbackInfoLeds(String messageTopic, String newState) {
  for (byte i=0; i<InfoLedCount; i++) {
    infoLeds[i].callback(messageTopic, newState);
  }
}

void checkInfoLeds() {
  for (byte i=0; i<InfoLedCount; i++)
    infoLeds[i].check();
}


// LED SEGMENTS

LedSegment ledSegments[LedSegmentCount];

void initLedSegments() {
  for (byte i=0; i<LedSegmentCount; i++)
    ledSegments[i].begin(LedSegmentLedOffsets[i], LedSegmentLedCounts[i], LedSegmentTopics[i], LedSegmentMemoryAddresses[i], &mqtt);
}

void callbackLedSegments(String messageTopic, String newState) {
  for (byte i=0; i<LedSegmentCount; i++) {
    ledSegments[i].callback(messageTopic, newState);
  }
}


// ------------------------------------ SETUP ------------------------------------

void setup()
{
 if(DEBUG)
    Serial.begin(115200);
  Serial.println(F("POWER ON."));

  EEPROM.begin(EEPROM_SIZE);

  mqttClient.setServer(MqttServerIp, MqttServerPort);
  mqttClient.setCallback(callback);
  mqtt.begin(&mqttClient, String(MqttTopic));

  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
  Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.

  WiFi.onEvent(WiFiEvent);

  ETH.begin();
  ETH.config(local_IP, gateway, subnet, gateway, gateway);
  
  Serial.println(F("Ready"));
}


// ------------------------------------ LOOP ------------------------------------


void loop()
{
  // check if ethernet is connected
  if (eth_connected) {
    // now take care of MQTT client...
    if (!mqttClient.connected()) {
      reconnect();
    } else {
      mqttClient.loop();
      healthPing(false);
      checkMotionSensors();
      checkButtons();
      checkInfoLeds();
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(MqttClientName)) {
    //if (mqttClient.connect(MqttClientName, MqttUsername, MqttPassword) { // if credentials is needed
      Serial.println("connected");
      if (!mqttHasBeenInitializedBefore)
        initialMqttInit();
      mqttHasBeenInitializedBefore = true;
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void initialMqttInit() {
  publishInitialStatus();
  initMotionSensors();
  initButtons();
  initDigitalStateOutputs();
  initInfoLeds();
  initLedSegments();
}

void publishInitialStatus() {
  String onlineText = String(MqttTopic);
  onlineText.concat(" online.");
  mqtt.publishMessage("status", "online", ip2Str(ETH.localIP()), onlineText);
  healthPing(true);
}

void healthPing(bool force) {
  if (millis()-lastHealthPing < HEALTH_PING_DELAY_MS && !force)
    return;
  lastHealthPing = millis();
  Serial.println(F("Starting health ping... "));

  String payload = "isAlive_";
  payload.concat(String(random(65535)));
  mqtt.publishMessage("health", "health_ping", payload, "");

  Serial.println(F("end."));
}

String ip2Str(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++) {
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  }
  return s;
}
