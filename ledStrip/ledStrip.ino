#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include "mqttClient.hpp"
#include "ledStrip.hpp"
#include "array.hpp"

array<const char*, 4> topics = {"/woonkamer/ledstrip/brightness", "/woonkamer/ledstrip", "/woonkamer/ledstrip/rgb/command", "/woonkamer/ledstrip/color/temp"};
WiFiClient espClient;
mqttClient client("SSID", "WPA", "192.168.178.74", topics, espClient);

ledStrip led(D2, D1, D3, client);

void callback(char* topic, byte* payload, unsigned int length) {
    static String message;
    for (int i = 0; i < length; i++) {
        message.concat((char)payload[i]);
    }
    client.notifyListeners(message, topic);
    message = "";  
}
 
void setup() {
  client.addListener(led);
  client.setupWifi();
  client.setupConnections();

  ArduinoOTA.setHostname("Led Strip");
  ArduinoOTA.setPassword((const char *)"OTA-PASS");
  ArduinoOTA.begin();
  }
 
void loop() {
  ArduinoOTA.handle();

  client.checkForMessages();
  led.fadeLights();
}
