/*
 Copyright (c) 2014 TechBubble Technologies and other Contributors.
 Developer:  Adam Milton-Barker (adammiltonbarker.com)
*/

extern "C" {
  #include "user_interface.h"
}

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h> 
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

ESP8266WebServer server;
WiFiClientSecure espClient;
PubSubClient client(espClient);

String deviceID = "Your JumpWay Device ID";
char* deviceName = "Your JumpWay Device Name";
String sensorType = "Your JumpWay Sensor Type"; 
int sensorID = "Your JumpWay Sensor ID";
String zoneID = "Your JumpWay Zone ID"; 
String locationID = "Your JumpWay Location ID";

String eSSID = "Your Wifi SSID (Network Name)";
String ePass = "Your Wifi Pin"; 

const char* mqttServer = "iot.techbubbletechnologies.com";
int  mqttPort = 8883; 
char mqttUsername[100]; 
char mqttPassword[100];
const char* mqttUsernameP = "Your JumpWay MQTT Username";
const char* mqttPasswordP = "Your JumpWay MQTT Password";

float humidity, temperature;
unsigned long previousMillis = 0;   
const long interval = 2000; 
const int powerPin = 5, sensorPin = 4;

char charBuf[50];
char willTopic[50];
int  passwordAttempts = 0; 

DHT dht(sensorPin, DHT11,15);

bool checkWifiConnection(void) {
  int connectAttempt = 0;
  while ( connectAttempt < 20 ) {
    if (WiFi.status() == WL_CONNECTED) { return true; } 
    delay(500);
    connectAttempt++;
  }
  return false;
} 

void gettemperature() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    humidity = dht.readHumidity();  
    temperature = dht.readTemperature();  
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    Serial.println(String(temperature));
    Serial.println(String(humidity));
  }
}

void publishToDeviceStatus(const char* data){
  String statusTopic = locationID+"/Devices/"+zoneID+"/"+deviceID+"/Status";
  statusTopic.toCharArray(charBuf, 50);
  client.publish(charBuf,data);
}

void publishToDeviceSensors(const char* data){
  String sensorsTopic = locationID+"/Devices/"+zoneID+"/"+deviceID+"/Sensors";
  char charBuf[50];
  sensorsTopic.toCharArray(charBuf, 50);
  client.publish(charBuf,data);
}

void publishToDeviceHardware(const char* data){
  String statusTopic = locationID+"/Devices/"+zoneID+"/"+deviceID+"/Hardware";
  statusTopic.toCharArray(charBuf, 50);
  client.publish(charBuf,data);
}

void reconnect() {
  strcpy(mqttUsername, mqttUsernameP);
  strcpy(mqttPassword, mqttPasswordP);
  while (!client.connected()) {
    Serial.println(F("Attempting connection to TechBubble IoT JumpWay..."));
    String willTopicString = locationID+"/Devices/"+zoneID+"/"+deviceID+"/Status";
    willTopicString.toCharArray(willTopic, 50);
    if (client.connect(deviceName,mqttUsername,mqttPassword, willTopic, 0, 0, "OFFLINE")) {
      Serial.println(F("Connected to TechBubble IoT JumpWay!"));
      publishToDeviceStatus("ONLINE");
    } else {
      passwordAttempts = passwordAttempts + 1;
      if(passwordAttempts == 5){
        Serial.println(F("5 FAILED LOGINS!")); 
        Serial.println(F(""));  
      } else {
        Serial.println(F("... trying again in 5 seconds"));
        delay(5000);        
      }
    }
  }
}

void setup() {   
  Serial.begin(115200);
  delay(10);
  Serial.println(F("System Initiating..."));
  pinMode(powerPin, OUTPUT);
  client.setServer(mqttServer,mqttPort);
  WiFi.begin(eSSID.c_str(), ePass.c_str());      
  if (checkWifiConnection()) { 
    Serial.println(F("Successfully connected to WiFi...")); 
    return;  
  } 
}

void loop() {  
     
  if (!client.connected()) {
    reconnect();
  }
  
  digitalWrite(powerPin, HIGH);
  delay(1000);
  
  dht.begin();
  gettemperature();
  delay(1000);

  float vdd = ESP.getVcc() / 1000.0;
  String jsonStatString = "{\"STAT\":\"BATTERY\",\"VALUE\":\""+String(vdd)+"\"}";
  char charBuf[100];
  jsonStatString.toCharArray(charBuf, 100);
  publishToDeviceHardware(charBuf); 
  Serial.println(jsonStatString);
  delay(1000);
  
  String jsonString = "{\"Sensor\":\"Temperature Sensor\",\"SensorID\":\""+sensorID+"\",\"SensorValue\": \""+String(temperature)+"\"}";
  char charBuff[95];
  jsonString.toCharArray(charBuff, 95);
  publishToDeviceSensors(charBuff); 
  Serial.println(jsonString);
  
  String jsonStrings = "{\"Sensor\":\"Humidity\",\"SensorID\":\""+sensorID+"\",\"SensorValue\": \""+String(humidity)+"\"}";
  char charBufff[89];
  jsonStrings.toCharArray(charBufff, 89);
  publishToDeviceSensors(charBufff); 
  Serial.println(jsonStrings);
  
  Serial.println(F("Going to sleep..."));
  Serial.flush();
  //ESP.deepSleep(0.5 * 60 * 1000 * 1000);
  wifi_set_sleep_type(LIGHT_SLEEP_T);
  delay(180000);
}