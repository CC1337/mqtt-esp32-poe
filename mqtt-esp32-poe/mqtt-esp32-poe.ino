/*
 * This sketch was written for Olimex ESP32-POE[-ISO].
 * Various sensors or actuators can be connected to be integrated into your smart home using MQTT (which integrates easily into f.e. ioBroker using the MQTT adapter).
 * Environment specific configuration will likely be done in setup.cpp and config.h only.
 * 
 * MQTT and ETH setup based on https://github.com/zorce/ESP32-POE_MQTT_example/blob/master/ESP32-POE_MQTT.ino
 */

#include "setup.cpp"

#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_PHY_POWER 12

#include <ETH.h>


// Initializations of network clients
IPAddress local_IP(NETWORK_LOCAL_IP);
IPAddress gateway(NETWORK_GATEWAY);
IPAddress subnet(NETWORK_SUBNET);

WiFiClient espClient;
PubSubClient mqttClient(espClient);
MqttPubSub mqtt;

const char* MqttTopic = MQTT_TOPIC;
const char* MqttServerIp = MQTT_SERVER_IP;
const short MqttServerPort = MQTT_SERVER_PORT;
const char* MqttClientName = MQTT_SERVER_CLIENTNAME;
const char* MqttUsername = MQTT_SERVER_USERNAME;
const char* MqttPassword = MQTT_SERVER_PASSWORD;

static bool eth_connected = false;
uint64_t chipid;
unsigned long lastHealthPing = millis();
boolean mqttHasBeenInitializedBefore = false;


// ------------------------------------ MODULES SETUP ------------------------------------

// MOTION SENSORS

MotionSensor motionSensors[MotionSensorCount];

void initMotionSensors() {
  for (byte i=0; i<MotionSensorCount; i++)
    motionSensors[i].begin(MotionSensorPins[i], MotionSensorTopics[i], &mqtt);
}

void loopMotionSensors() {
  for (byte i=0; i<MotionSensorCount; i++)
    motionSensors[i].loop();
}


// BUTTONS

Button buttons[ButtonCount];

void initButtons() {
  for (byte i=0; i<ButtonCount; i++)
    buttons[i].begin(ButtonPins[i], ButtonTopics[i], &mqtt);
}

void loopButtons() {
  for (byte i=0; i<ButtonCount; i++)
    buttons[i].loop();
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

void loopInfoLeds() {
  for (byte i=0; i<InfoLedCount; i++)
    infoLeds[i].loop();
}


// LED SEGMENTS

AddressableLeds leds;
AddressableLedPower ledPower;
LedSegment ledSegments[LedSegmentCount];

void initAddressableLeds() {
  ledPower.begin(ADDRESSABLE_LED_POWER_PIN);
  leds.begin(ADDRESSABLE_LED_PIN, ADDRESSABLE_LED_COUNT, &ledPower);
  FastLED.addLeds<ADDRESSABLE_LED_TYPE, ADDRESSABLE_LED_PIN>(leds.ledArray, ADDRESSABLE_LED_COUNT);
}

void initLedSegments() {
  initAddressableLeds();
  for (byte i=0; i<LedSegmentCount; i++)
    ledSegments[i].begin(LedSegmentLedOffsets[i], LedSegmentLedCounts[i], LedSegmentTopics[i], LedSegmentMemoryAddresses[i], &leds, &mqtt);
}

void callbackLedSegments(String messageTopic, String newState) {
  for (byte i=0; i<LedSegmentCount; i++) {
    ledSegments[i].callback(messageTopic, newState);
  }
}

void loopLedSegments() {
  for (byte i=0; i<LedSegmentCount; i++) {
    ledSegments[i].loop();
  }
  leds.show();
}



// ------------------------------------ MODULES HOOKUP POINTS ------------------------------------

// Put initialization of modules that might need MQTT reference here to make sure it's ready
void afterMqttInit() {
  initMotionSensors();
  initButtons();
  initDigitalStateOutputs();
  initInfoLeds();
  initLedSegments();
}

// Put methods here that should be called every main loop cycle while MQTT is connected
void everyLoop() {
  loopMotionSensors();
  loopButtons();
  loopInfoLeds();
  loopLedSegments();
}

// Put callbacks here that will receive all MQTT messages below the configured MqttTopic from config.h
void moduleMqttCallbacks(String messageTopic, String payload) {
  callbackDigitalStateOutputs(messageTopic, payload);
  callbackInfoLeds(messageTopic, payload);
  callbackLedSegments(messageTopic, payload);
}


// ------------------------------------ SETUP ------------------------------------

void setup()
{
 if(DEBUG)
    Serial.begin(115200);
  Serial.println(F("POWER ON."));

  EEPROM.begin(EEPROM_SIZE);

  mqttClient.setServer(MqttServerIp, MqttServerPort);
  mqttClient.setCallback(mqttCallback);
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
  if (eth_connected) {
    if (!mqttClient.connected()) {
      mqttReconnect();
    } else {
      mqttClient.loop();
      healthPing(false);
      everyLoop();
    }
  }
}


// ------------------------------------ BASIC FUNCTIONS ------------------------------------

// As this device needs to be connected because all functionality depends on MQTT, this method id blocking if not connected
void mqttReconnect() {
  while (!mqttClient.connected()) {
    Serial.print(F("Starting MQTT connection..."));
    if (mqttClient.connect(MqttClientName)) {
    //if (mqttClient.connect(MqttClientName, MqttUsername, MqttPassword) { // optional with credentials
      Serial.println(F(" connected."));
      if (!mqttHasBeenInitializedBefore) {
          publishInitialStatus();
          afterMqttInit();
      }
      mqttHasBeenInitializedBefore = true;
    } else {
      Serial.print(F(" failed, rc="));
      Serial.print(mqttClient.state());
      Serial.println(F(" will try again in 5 seconds"));

      delay(5000);
    }
  }
}

// Every message to a topic we subscribed to will end up here, logged and forwarded to moduleMqttCallbacks
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String payloadString;
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    payloadString.concat((char)payload[i]);
  }
  Serial.println();

  moduleMqttCallbacks(String(topic), payloadString);
}

// On boot, publish IP
void publishInitialStatus() {
  String onlineText = String(MqttTopic);
  onlineText.concat(" online.");
  mqtt.publishMessage("status", "online", ip2Str(ETH.localIP()), onlineText);
  healthPing(true);
}

// Every HEALTH_PING_DELAY_MS ms (config.h), there will be a message with random number to be able monitoring the online state of this device
void healthPing(bool force) {
  if (millis()-lastHealthPing < HEALTH_PING_DELAY_MS && !force)
    return;
  lastHealthPing = millis();

  String payload = "isAlive_";
  payload.concat(String(random(65535)));
  mqtt.publishMessage("health", "health_ping", payload, "");
}

// Ethernet setup
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

// ------------------------------------ POSSIBLY SHARED FUNCTIONS ------------------------------------

// For nice written boolean values instead of 0 and 1 in MQTT messages
String bool2Str(bool buhl) {
  return buhl ? "true" : "false";
}

// Does what the name tells you...
String ip2Str(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++) {
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  }
  return s;
}
