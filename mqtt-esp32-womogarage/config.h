
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
 * ---------- TOPIC ASSIGNMENTS ----------
 */


const byte StateTopicCount = 2;
const char StateTopics[StateTopicCount][TOPIC_MAX_LENGTH] =
{
  VORGARTEN_STRAHLER_SUBTOPIC,
  WOHNMOBIL_STECKDOSE
};

/*
 * ---------- PINS ----------
 */

#define BWM_VORGARTEN_PIN 10
#define BWM_GARTEN_PIN 11


/*
 * ---------- TIMINGS ----------
 */

// Health Ping
#define HealthPingDelayMs 60000




#endif
