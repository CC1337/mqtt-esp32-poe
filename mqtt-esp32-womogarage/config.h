// Please also check setup.cpp

#ifndef config_h
#define config_h


/*
 * ---------- DEBUG ----------
 */

#define DEBUG true


/*
 * ---------- NETWORK ----------
 */

#define NETWORK_LOCAL_IP 192, 168, 0, 95
#define NETWORK_GATEWAY 192, 168, 0, 1
#define NETWORK_SUBNET 255, 255, 255, 0


/*
 * ---------- MQTT BASE SETUP ----------
 */

#define MQTT_TOPIC "ESP32-Wohnmobilgarage"
#define MQTT_SERVER_IP "192.168.0.90"       // IP address of the MQTT broker
#define MQTT_SERVER_PORT 1883               // IP port of the MQTT broker
#define MQTT_SERVER_CLIENTNAME MQTT_TOPIC;
#define MQTT_SERVER_USERNAME NULL
#define MQTT_SERVER_PASSWORD NULL


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
#define TASTER_LICHT_VORGARTEN_SUBTOPIC "TasterLichtVorgarten"
#define TASTER_STECKDOSE_WOHNMOBIL_SUBTOPIC "TasterWohnmobilSteckdose"


/*
 * ---------- PINS ----------
 */

#define BWM_VORGARTEN_PIN 32
#define BWM_GARTEN_PIN 33
#define VORGARTEN_STRAHLER_PIN 15
#define WOHNMOBIL_STECKDOSE_PIN 16
#define LED_TASTER_LICHT_VORGARTEN_PIN 4
#define LED_TASTER_STECKDOSE_WOHNMOBIL_PIN 5
#define TASTER_LICHT_VORGARTEN_PIN 34
#define TASTER_STECKDOSE_WOHNMOBIL_PIN 14
#define ADDRESSABLE_LED_PIN 2
#define ADDRESSABLE_LED_POWER_PIN 13


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
 * ---------- ADDRESSABLE LEDS ----------
 */

#define ADDRESSABLE_LED_COUNT 60
#define ADDRESSABLE_LED_TYPE UCS1903B
#define ADDRESSABLE_LED_POWER_PIN_INVERT false
#define ADDRESSABLE_LEDS_POWER_OFF_DELAY_MS 5000



/*
 * ---------- TIMINGS ----------
 */

// Button
#define BUTTON_LONG_PRESS_MS 1000
#define BUTTON_RELEASE_MAX_DELAY 800
#define BUTTON_PRESS_MIN_MS 80

// Health Ping
#define HEALTH_PING_DELAY_MS 60000

#endif
