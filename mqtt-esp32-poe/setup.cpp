// Please also check config.h

#include <EEPROM.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "config.h"
#include "MqttPubSub.h"
#include "MotionSensor.h"
#include "Button.h"
#include "DigitalStateOutput.h"
#include "InfoLed.h"
#include "AddressableLeds.h"
#include "AddressableLedPower.h"
#include "LedSegment.h"




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
  false,
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
 * ---------- BUTTON TOPIC ASSIGNMENTS ----------
 */

const byte ButtonCount = 2;
const char ButtonTopics[ButtonCount][TOPIC_MAX_LENGTH] =
{
  TASTER_LICHT_VORGARTEN_SUBTOPIC,
  TASTER_STECKDOSE_WOHNMOBIL_SUBTOPIC
};
const byte ButtonPins[ButtonCount] =
{
  TASTER_LICHT_VORGARTEN_PIN,
  TASTER_STECKDOSE_WOHNMOBIL_PIN
};


/*
 * ---------- ADDRESSABLE LED SEGMENTS ----------
 */

const byte LedSegmentCount = 6;
const char LedSegmentTopics[LedSegmentCount][TOPIC_MAX_LENGTH] =
{
  LED_GARTEN_SUBTOPIC,
  LED_VG_VERT_4_SUBTOPIC,
  LED_VG_VERT_3_SUBTOPIC,
  LED_VG_VERT_2_SUBTOPIC,
  LED_VG_VERT_1_SUBTOPIC,
  LED_VG_SEITE_SUBTOPIC,
};
const byte LedSegmentLedOffsets[LedSegmentCount] =
{
  0,
  14,
  26,
  38,
  50,
  62,
};
const byte LedSegmentLedCounts[LedSegmentCount] =
{
  14,
  12,
  12,
  12,
  12,
  71
};
// 3 byte needed for each
const byte LedSegmentMemoryAddresses[LedSegmentCount] =
{
  LED_GARTEN_MEMORY_ADDRESS,
  LED_VG_VERT_4_MEMORY_ADDRESS,
  LED_VG_VERT_3_MEMORY_ADDRESS,
  LED_VG_VERT_2_MEMORY_ADDRESS,
  LED_VG_VERT_1_MEMORY_ADDRESS,
  LED_VG_SEITE_MEMORY_ADDRESS
};
