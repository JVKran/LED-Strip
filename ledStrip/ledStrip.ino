#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include "mqttClient.hpp"
#include <ArduinoOTA.h>
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
  ArduinoOTA.begin();
  client.addListener(led);
  client.setupWifi();
  client.setupConnections();
}
 
void loop() {
  ArduinoOTA.handle();
  client.checkForMessages();
}
