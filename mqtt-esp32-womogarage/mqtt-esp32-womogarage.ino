#include <SPI.h>
#include <WiFiClientSecure.h>
#include <EEPROM.h>
#include <PubSubClient.h>
#include <ESPmDNS.h>
#include "config.h"

#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_PHY_POWER 12
#include <ETH.h>

IPAddress local_IP(192, 168, 0, 95);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192, 168, 0, 1); 

const char* mqttServer = "192.168.0.90";
const char* mqttTopic = "ESP32-Wohnmobilgarage";

unsigned long lastEthernetMaintain = millis();
unsigned long lastHealthPing = millis();
bool wasOffline = false;

WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

void hardwareReset() {
  Serial.println("HARDWARE RESET");
  digitalWrite(RESET_PIN, LOW);
}

// --------------------------- SETUP ---------------------------

void setup()
{
  digitalWrite(RESET_PIN, HIGH);
  pinMode(RESET_PIN, OUTPUT);
  
  if(DEBUG)
    Serial.begin(115200);

  Serial.println(F("POWER ON."));
  
  startEthernet();

  if (EEPROM.read(RESETINFO_ADDRESS) != 1 && !isOnline()) {
    Serial.println(F("First start attempt after power on... reseting."));
    delay(5000);
    EEPROM.write(RESETINFO_ADDRESS, 1);
    hardwareReset();
  }
  EEPROM.write(RESETINFO_ADDRESS, 0);

  delay(1000);

  pinMode(BWM_VORGARTEN_PIN, INPUT_PULLUP);
  pinMode(BWM_GARTEN_PIN, INPUT_PULLUP);

  healthPing(true);
  
  Serial.println(F("Ready"));
}

void loop()
{
  checkEthernet();
  mqttClient.loop();
  healthPing(false);

  //TODO

  delay(10);
}

void checkEthernet() {
  if (millis()-lastEthernetMaintain < 60000)
    return;
  Serial.println(F("Ethernet check starts."));
  if (!isOnline())
    wasOffline = true;
  lastEthernetMaintain = millis();
  if (isOnline() && wasOffline) {
    wasOffline = false;
    String onlineText = String("Wieder online. IP: ");
    onlineText.concat(ip2Str(WiFi.localIP()));
    Serial.println(onlineText);
      mqttPublish("status", "online_again", ip2Str(WiFi.localIP()), onlineText);
  } else if (!isOnline()) {
    Serial.println(F("Ethernet connection failed."));
  }
}

void mqttPublish(String subtopic, String action, String payload, String fullMessage) {
  String message = "{ \"action\": \"";
  message.concat(action.c_str());
  message.concat("\", \"payload\": \"");
  message.concat(payload.c_str());
  message.concat("\", \"fullMessage\": \"");
  message.concat(fullMessage.c_str());
  message.concat("\"}");
  subtopic = "/" + subtopic;
  subtopic = mqttTopic + subtopic;
  mqttConnect();
  if(mqttClient.publish(subtopic.c_str(), message.c_str())) {
    Serial.println("Publish message success: " + message);
  } else {
    Serial.println("Could not send message: " + message);
  }
  delay(300);
  mqttDisconnect();
}

void startEthernet() {
  Serial.print(F("Starting ethernet and connecting to MQTT server... "));
  //WiFi.begin(mac, ip);
  ETH.begin();
  WiFi.config(local_IP, primaryDNS, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  mqttClient.setServer(mqttServer, 1883);   
  if (!mqttConnect()) 
  {
    Serial.println("failed.");
  }
  else {
    Serial.println(WiFi.localIP());
    wasOffline = false;
  }

  mqttDisconnect();

  String onlineText = String("Online. IP: ");
  onlineText.concat(ip2Str(WiFi.localIP()));

  mqttPublish("status", "online_initial", ip2Str(WiFi.localIP()), onlineText);
}

bool isOnline() {
  bool res = mqttConnect();
  mqttDisconnect();
  return res;
}

bool mqttConnect() {
  return mqttClient.connect(mqttTopic);
}

bool mqttDisconnect() {
   mqttClient.disconnect();
}


void healthPing(bool force) {
  if (millis()-lastHealthPing < HealthPingDelayMs && !force)
    return;
  lastHealthPing = millis();
  Serial.print(F("Starting health ping... "));
  
  mqttPublish("status", "health_ping", "isAlive", "");
  
  Serial.println(F("end."));
}

String ip2Str(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++) {
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  }
  return s;
}
