// Please also check setup.cpp

#ifndef config_h
#define config_h


/*
 * ---------- DEBUG ----------
 */

// true = serial output enabled
#define DEBUG true

// switch IP settings for DEV and PROD.
#define OUTDOOR_VLAN false


/*
 * ---------- NETWORK ----------
 */

#if OUTDOOR_VLAN
  #define NETWORK_LOCAL_IP 192, 168, 2, 4
  #define NETWORK_GATEWAY 192, 168, 2, 13
  #define NETWORK_SUBNET 255, 255, 255, 0
#else
  #define NETWORK_LOCAL_IP 192, 168, 0, 95
  #define NETWORK_GATEWAY 192, 168, 0, 1
  #define NETWORK_SUBNET 255, 255, 255, 0
#endif


/*
 * ---------- MQTT BASE SETUP ----------
 */

#if OUTDOOR_VLAN
  #define MQTT_TOPIC "ESP32-Wohnmobilgarage"    // Main name of this device
#else
  #define MQTT_TOPIC "ESP32-Dev"    // Main name of this device
#endif
#if OUTDOOR_VLAN
  #define MQTT_SERVER_IP "192.168.2.13"       // IP address of the MQTT broker
#else
  #define MQTT_SERVER_IP "192.168.0.90"       // IP address of the MQTT broker
#endif
#define MQTT_SERVER_PORT 1883                 // IP port of the MQTT broker
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
#define LED_VG_VERT_4_SUBTOPIC "LedVorgartenVertikal4"
#define LED_VG_SEITE_SUBTOPIC "LedVorgartenSeite"
#define LED_GARTEN_SUBTOPIC "LedGarten"
#define LED_TASTER_LICHT_VORGARTEN_SUBTOPIC "LedTasterLichtVorgarten"
#define LED_TASTER_STECKDOSE_WOHNMOBIL_SUBTOPIC "LedTasterWohnmobilSteckdose"
#define TASTER_LICHT_VORGARTEN_SUBTOPIC "TasterLichtVorgarten"
#define TASTER_STECKDOSE_WOHNMOBIL_SUBTOPIC "TasterWohnmobilSteckdose"


/*
 * ---------- PINS ----------
 * - MotionSensors have 3.3V output and they are set to only trigger short high on detection
 * - Buttons are connected to GND
 * - Outputs can be connected to MOSFET or Relay boards. For relay boards the output needs to be inverted. Pin low = Relay ON = MQTT state true if inverted = true
 * - Only one addressable LED pin and only white colored ones are implemented right now.
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

#define EEPROM_SIZE 21

#define VORGARTEN_STRAHLER_MEMORY_ADDRESS 1               // 1 byte needed
#define WOHNMOBIL_STECKDOSE_MEMORY_ADDRESS 2              // 1 byte needed
#define LED_VG_VERT_1_MEMORY_ADDRESS 3                    // 3 byte needed
#define LED_VG_VERT_2_MEMORY_ADDRESS 6                    // 3 byte needed
#define LED_VG_VERT_3_MEMORY_ADDRESS 9                    // 3 byte needed
#define LED_TASTER_LICHT_VORGARTEN_MEMORY_ADDRESS 10      // 1 byte needed
#define LED_TASTER_STECKDOSE_WOHNMOBIL_MEMORY_ADDRESS 11  // 1 byte needed
#define LED_VG_VERT_4_MEMORY_ADDRESS 12                   // 3 byte needed
#define LED_VG_SEITE_MEMORY_ADDRESS 15                    // 3 byte needed
#define LED_GARTEN_MEMORY_ADDRESS 18                      // 3 byte needed


/*
 * ---------- ADDRESSABLE LEDS ----------
 */

#define ADDRESSABLE_LED_COUNT 133
#define ADDRESSABLE_LED_TYPE UCS1903B                     // check FastLED docs for more
#define ADDRESSABLE_LED_POWER_PIN_INVERT false
#define ADDRESSABLE_LEDS_POWER_OFF_DELAY_MS 30000
#define ADDRESSABLE_LEDS_POWER_ON_DELAY_MS 800


/*
 * ---------- TIMINGS ----------
 */

// Button
#define BUTTON_LONG_PRESS_MS 1000
#define BUTTON_RELEASE_MAX_DELAY 800
#define BUTTON_PRESS_MIN_MS 80
#define BUTTON_TOPIC_RESET_DELAY 1000

// Health Ping
#define HEALTH_PING_DELAY_MS 60000

#endif
