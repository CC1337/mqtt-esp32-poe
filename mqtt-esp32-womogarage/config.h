
#ifndef config_h
#define config_h


/*
 * ---------- DEBUG ----------
 */

#define DEBUG true


/*
 * ---------- NETWORK ----------
 */

IPAddress local_IP(192, 168, 0, 95);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);


/*
 * ---------- MQTT BASE SETUP ----------
 */

const char* MqttTopic = "ESP32-Wohnmobilgarage";
const char* MqttServerIp = "192.168.0.90"; // IP address of the MQTT broker
const short MqttServerPort = 1883; // IP port of the MQTT broker
const char* MqttClientName = MqttTopic;
const char* MqttUsername = NULL;
const char* MqttPassword = NULL;


/*
 * ---------- SUBTOPICS ----------
 */

// Maxlength must be bigger as MqttTopic strlen + longest subtopic below
#define TOPIC_MAX_LENGTH 100

#define BWM_VORGARTEN_SUBTOPIC "BewegungsmelderVorgarten"
#define BWM_GARTEN_SUBTOPIC "BewegungsmelderGarten"
#define VORGARTEN_STRAHLER_SUBTOPIC "StrahlerVorgarten"
#define WOHNMOBIL_STECKDOSE "WohnmobilSteckdose"


/*
 * ---------- PINS ----------
 */

#define BWM_VORGARTEN_PIN 4
#define BWM_GARTEN_PIN 5
#define VORGARTEN_STRAHLER_PIN 13
#define WOHNMOBIL_STECKDOSE_PIN 14


/*
 * ---------- FLASH MEMORY ----------
 */

#define EEPROM_SIZE 10

#define VORGARTEN_STRAHLER_MEMORY_ADDRESS 1
#define WOHNMOBIL_STECKDOSE_MEMORY_ADDRESS 2


/*
 * ---------- OUTPUTSTATE TOPIC ASSIGNMENTS ----------
 */

const byte DigitalStateOutputCount = 2;
const char DigitalStateOutputTopics[DigitalStateOutputCount][TOPIC_MAX_LENGTH] =
{
  VORGARTEN_STRAHLER_SUBTOPIC,
  WOHNMOBIL_STECKDOSE_SUBTOPIC
};
const byte DigitalStateOutputPins[DigitalStateOutputCount] =
{
  VORGARTEN_STRAHLER_PIN,
  WOHNMOBIL_STECKDOSE_PIN
};
// 1 byte needed for each
const byte DigitalStateOutputMemoryAddresses[DigitalStateOutputCount] =
{
  VORGARTEN_STRAHLER_MEMORY_ADDRESS,
  WOHNMOBIL_STECKDOSE_MEMORY_ADDRESS
};


/*
 * ---------- MOTIONSENSOR TOPIC ASSIGNMENTS ----------
 */

const byte MotionSensorCount = 2;
const char MotionSensorTopics[MotionSensorCount][TOPIC_MAX_LENGTH] =
{
  BWM_VORGARTEN_SUBTOPIC,
  BWM_GARTEN_SUBTOPIC
};
const byte MotionSensorPins[MotionSensorCount] =
{
  BWM_VORGARTEN_PIN,
  BWM_GARTEN_PIN
};


/*
 * ---------- TIMINGS ----------
 */

// Health Ping
#define HealthPingDelayMs 60000


#endif
