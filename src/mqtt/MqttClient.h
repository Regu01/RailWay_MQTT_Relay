#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoLog.h>

class MqttClient {
public:
    MqttClient(const char* server, int port, const char* user, const char* password);
    void connect();
    void disconnect();
    void publish(const char* topic, const char* message);
    void subscribe(const char* topic);
    bool isConnected();
    void loop();

private:
    const char* _server;
    int _port;
    const char* _user;
    const char* _password;
    WiFiClient _wifiClient;
    PubSubClient _client;

    void reconnect();
};

#endif // MQTTCLIENT_H
