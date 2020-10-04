
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
#define WOHNMOBIL_STECKDOSE_SUBTOPIC "WohnmobilSteckdose"
#define LED_VG_VERT_1_SUBTOPIC "LedVorgartenVertikal1"
#define LED_VG_VERT_2_SUBTOPIC "LedVorgartenVertikal2"
#define LED_VG_VERT_3_SUBTOPIC "LedVorgartenVertikal3"
#define LED_TASTER_LICHT_VORGARTEN_SUBTOPIC "LedTasterLichtVorgarten"
#define LED_TASTER_STECKDOSE_WOHNMOBIL_SUBTOPIC "LedTasterWohnmobilSteckdose"


/*
 * ---------- PINS ----------
 */

#define BWM_VORGARTEN_PIN 4
#define BWM_GARTEN_PIN 5
#define VORGARTEN_STRAHLER_PIN 13
#define WOHNMOBIL_STECKDOSE_PIN 14
#define LED_TASTER_LICHT_VORGARTEN_PIN 15
#define LED_TASTER_STECKDOSE_WOHNMOBIL_PIN 16


/*
 * ---------- FLASH MEMORY ----------
 */

#define EEPROM_SIZE 20

#define VORGARTEN_STRAHLER_MEMORY_ADDRESS 1               // 1 byte needed
#define WOHNMOBIL_STECKDOSE_MEMORY_ADDRESS 2              // 1 byte needed
#define LED_VG_VERT_1_MEMORY_ADDRESS 3                    // 3 byte needed
#define LED_VG_VERT_2_MEMORY_ADDRESS 6                    // 3 byte needed
#define LED_VG_VERT_3_MEMORY_ADDRESS 9                    // 3 byte needed
#define LED_TASTER_LICHT_VORGARTEN_MEMORY_ADDRESS 10      // 1 byte needed
#define LED_TASTER_STECKDOSE_WOHNMOBIL_MEMORY_ADDRESS 11  // 1 byte needed

/*
 * ---------- DIGITALSTATEOUTPUT TOPIC ASSIGNMENTS ----------
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
const byte DigitalStateOutputInverted[DigitalStateOutputCount] =
{
  true,
  false
};
// 1 byte needed for each
const byte DigitalStateOutputMemoryAddresses[DigitalStateOutputCount] =
{
  VORGARTEN_STRAHLER_MEMORY_ADDRESS,
  WOHNMOBIL_STECKDOSE_MEMORY_ADDRESS
};


/*
 * ---------- INFOLED TOPIC ASSIGNMENTS ----------
 */

const byte InfoLedCount = 2;
const char InfoLedTopics[DigitalStateOutputCount][TOPIC_MAX_LENGTH] =
{
  LED_TASTER_LICHT_VORGARTEN_SUBTOPIC,
  LED_TASTER_STECKDOSE_WOHNMOBIL_SUBTOPIC
};
const byte InfoLedPins[InfoLedCount] =
{
  LED_TASTER_LICHT_VORGARTEN_PIN,
  LED_TASTER_STECKDOSE_WOHNMOBIL_PIN
};
// 1 byte needed for each
const byte InfoLedMemoryAddresses[InfoLedCount] =
{
  LED_TASTER_LICHT_VORGARTEN_MEMORY_ADDRESS,
  LED_TASTER_STECKDOSE_WOHNMOBIL_MEMORY_ADDRESS
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
 * ---------- LED SEGMENTS ----------
 */

const byte LedSegmentCount = 3;
const char LedSegmentTopics[LedSegmentCount][TOPIC_MAX_LENGTH] =
{
  LED_VG_VERT_3_SUBTOPIC,
  LED_VG_VERT_2_SUBTOPIC,
  LED_VG_VERT_1_SUBTOPIC
};
const byte LedSegmentLedOffsets[LedSegmentCount] =
{
  0,
  10,
  20
};
const byte LedSegmentLedCounts[LedSegmentCount] =
{
  10,
  10,
  10
};
// 3 byte needed for each
const byte LedSegmentMemoryAddresses[LedSegmentCount] =
{
  LED_VG_VERT_3_MEMORY_ADDRESS,
  LED_VG_VERT_2_MEMORY_ADDRESS,
  LED_VG_VERT_1_MEMORY_ADDRESS
};


/*
 * ---------- TIMINGS ----------
 */

// Health Ping
#define HEALTH_PING_DELAY_MS 60000

#endif
