# IoT JumpWay Arduino ESP8266 Basic MQTT Example

![IoT JumpWay TTM ESP8266 Arduino Device](../../Images/TTM/banner.png)

## Introduction

Here you will find sample device scripts for connecting ESP8266 to the TechBubble Technologies IoT JumpWay using the Arduino IDE. The codes allow you to set up a basic device that allows communication via the TechBubble IoT JumpWay using MQTT. Once you understand how it works you are free to add as many actuators and sensors to your device and modify your code accordingly.

## Hardware Requirements

1. ESP8266.

## Software requirements

1. ESP8266WiFi
2. PubSubClient MQTT  
3. ArduinoJson
4. WiFiClientSecure

## Connection Credentials

- Follow the [TechBubble Technologies IoT JumpWay Developer Program (BETA) Location Device Doc-](https://github.com/TechBubbleTechnologies/IoT-JumpWay-Docs/blob/master/4-Location-Devices.md "TechBubble Technologies IoT JumpWay Developer Program (BETA) Location Device Doc") to set up your device. 

- Add your WiFi credentials to the following code which will allow your ESP8266 to connect to the internet.

```
	const char* ssid = "YourSSID";
    const char* password = "YourWiFiPassword";
```

- Retrieve your connection credentials and update the config.json file with your new connection  credentials and sensor setting.

```
    String locationID = "YourLocationID";
    String zoneID = "YourZoneID";
    String deviceID = "YourDeviceID";
    char deviceName[] = "YourDeviceName"; 
    char mqttUsername[]   = "YourDeviceMQTTUsername"; 
    char mqttPassword[]  = "YourDeviceMQTTPassword"; 
```

## IoT JumpWay Arduino ESP8266 Examples Bugs/Issues

Please feel free to create issues for bugs and general issues you come accross whilst using the IoT JumpWay Arduino ESP8266 Examples. You may also use the issues area to ask for general help whilst using the IoT JumpWay Arduino ESP8266 Examples in your IoT projects.

## IoT JumpWay Arduino ESP8266 Contributors

- [Adam Milton-Barker, TechBubble Technologies Founder](https://github.com/AdamMiltonBarker "Adam Milton-Barker, TechBubble Technologies Founder")