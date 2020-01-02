#ifndef __CONNECTIONS_HPP
#define __CONNECTIONS_HPP

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "array.hpp"

class messageListener {
    public:
        virtual void messageReceived(const String & receivedMessage, const char* topic = "") = 0;
};

void callback(char* topic, byte* payload, unsigned int length);

class mqttClient {
    //Led will be red when connecting to WiFi, yellow when connecting to MQTT Broker
    //and colorless when functioning. After succesfully connecting to the broker, 
    //the led will be green for 2 seconds.
    private:
        char* ssid;
        char* password;
        char* mqttServer;
        
        array<const char*, 4> topics;
        PubSubClient client;
        const bool retainedMessages;
        const uint8_t qosLevel;
    public:
        messageListener * listeners[20] = {};
        uint8_t amountOfListeners = 0;

        mqttClient(char* ssid, char* password, char* mqttServer, array<const char*, 4> topics, WiFiClient & espClient, const bool retainedMessages = true, const uint8_t qosLevel = 1);

        void addListener(messageListener & listener);
        void notifyListeners(const String & mesage, const char* topic);

        void setupWifi();
        void setupConnections();
        void reconnect();

        void checkForMessages();
        void sendMessage(const char* topic, const char* messageToSend);
};

#endif //__CONNECTIONS_HPP