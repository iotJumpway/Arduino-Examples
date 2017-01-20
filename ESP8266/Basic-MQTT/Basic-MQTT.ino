/*
  Copyright (c) 2016 TechBubble Technologies and other Contributors.

  All rights reserved. This program and the accompanying materials
  are made available under the terms of the Eclipse Public License v1.0
  which accompanies this distribution, and is available at
  http://www.eclipse.org/legal/epl-v10.html 

  Contributors:
  
  Adam Milton-Barker - TechBubble Technologies Limited
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h> 
#include <ArduinoJson.h>

WiFiClientSecure espClient;
PubSubClient client(espClient);

char charBuf[50];
 
const int actuator1Pin = 2;

const char* ssid = "YourSSID";
const char* password = "YourWiFiPassword";

const char* mqtt_server = "iot.techbubbletechnologies.com";
int  mqttPort = 8883;

String locationID = "YourLocationID";
String zoneID = "YourZoneID";
String deviceID = "YourDeviceID";
char deviceName[] = "YourDeviceName"; 
char mqttUsername[]   = "YourDeviceMQTTUsername"; 
char mqttPassword[]  = "YourDeviceMQTTPassword"; 

long lastMsg = 0;
char msg[50];
int value = 0;

void setupWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String readString = ""; 
  for (int i = 0; i < length; i++) {
    readString += (char)payload[i];
  }
  Serial.println(readString);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(readString);
  String CommandValue = root["CommandValue"];
  Serial.println(CommandValue);
  if(CommandValue=="ON"){
    Serial.println("TURNING ON");
    digitalWrite(actuator1Pin, HIGH);
  }else{
    Serial.println("TURNING OFF");
    digitalWrite(actuator1Pin, LOW);
  }
  Serial.println();
}

void subscribeToDeviceCommands(){
  String commandTopic = locationID+"/Devices/"+zoneID+"/"+deviceID+"/Commands";
  commandTopic.toCharArray(charBuf, 50);
  client.subscribe(charBuf);
}

void publishToDeviceStatus(const char* data){
  String statusTopic = locationID+"/Devices/"+zoneID+"/"+deviceID+"/Status";
  statusTopic.toCharArray(charBuf, 50);
  client.publish(charBuf,data);
}

void publishToDeviceSensors(const char* data){
  String sensorsTopic = locationID+"/Devices/"+zoneID+"/"+deviceID+"/Sensors";
  sensorsTopic.toCharArray(charBuf, 50);
  client.publish(charBuf,data);
}

void publishToDeviceActuators(const char* data){
  String actuatorsTopic = locationID+"/Devices/"+zoneID+"/"+deviceID+"/Actuators";
  actuatorsTopic.toCharArray(charBuf, 50);
  client.publish(charBuf,data);
}

void publishToDeviceWarnings(const char* data){
  String warningsTopic = locationID+"/Devices/"+zoneID+"/"+deviceID+"/Warnings";
  warningsTopic.toCharArray(charBuf, 50);
  client.publish(charBuf,data);
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting connection to TechBubble IoT JumpWay...");
    if (client.connect(deviceName, mqttUsername, mqttPassword)) {
      Serial.println("Connected to TechBubble IoT JumpWay!");
      publishToDeviceStatus("ONLINE");
      subscribeToDeviceCommands();
    } else {
      Serial.print("Failed to connect to TechBubble IoT JumpWay, rc=");
      Serial.print(client.state());
      Serial.println("... trying again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(actuator1Pin, OUTPUT);   
  Serial.begin(115200);
  setupWiFi();
  client.setServer(mqtt_server,mqttPort);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
  }
}
