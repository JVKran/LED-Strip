#include "mqttClient.hpp"

mqttClient::mqttClient(char* ssid, char* password, char* mqttServer, array<const char*, 4> topics, WiFiClient & espClient, const bool retainedMessages, const uint8_t qosLevel):
    ssid(ssid),
    password(password),
    mqttServer(mqttServer),
    topics(topics),
    client(espClient),
    retainedMessages(retainedMessages),
    qosLevel(qosLevel)
{
    WiFi.mode(WIFI_STA);
}

void mqttClient::addListener(messageListener & listener){
    if(amountOfListeners <= 19){
        listeners[amountOfListeners] = &listener;
        amountOfListeners++;
    }
}

void mqttClient::notifyListeners(const String & message, const char* topic){
	for (int i = 0; i < amountOfListeners; i++){
        listeners[i]->messageReceived(message, topic);
    }
}

void mqttClient::setupWifi() {
    delay(10);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void mqttClient::setupConnections(){
    client.setServer(mqttServer, 1883);
    client.setCallback(callback);
    for(const char* & topic : topics){
        client.subscribe(topic, qosLevel);
    }
    reconnect();                //Establish a connection by signing in with credentials.
}

void mqttClient::reconnect() {
    while (!client.connected()) {
        if (client.connect("LedStripClient", "LedStripClient", "Snip238!")) {
            for(const char* & topic : topics){
                client.subscribe(topic, qosLevel);
            }
            for (int i = 0; i < amountOfListeners; i++){
                listeners[i]->messageReceived("CONNECTED");
            }
            break;
        } else {
            delay(500);
        }
    }
}

void mqttClient::checkForMessages(){
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}

void mqttClient::sendMessage(const char* topic, const char* messageToSend){
    client.publish(topic, messageToSend, retainedMessages);
}
