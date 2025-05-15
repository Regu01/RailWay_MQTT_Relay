#include "MqttClient.h"

MqttClient::MqttClient(const char* server, int port, const char* user, const char* password)
    : _server(server), _port(port), _user(user), _password(password), _client(_wifiClient) {
    _client.setServer(_server, _port);
}

void MqttClient::connect() {
    Log.notice("Connecting to MQTT server: %s:%d" CR, _server, _port);
    reconnect();
}

void MqttClient::reconnect() {
    while (!_client.connected()) {
        Log.notice("Attempting MQTT connection..." CR);
        if (_client.connect("ESP32Client", _user, _password)) {
            Log.notice("Connected to MQTT broker." CR);
        } else {
            Log.error("Failed to connect to MQTT. Retrying in 5 seconds..." CR);
            delay(5000);
        }
    }
}

void MqttClient::disconnect() {
    if (_client.connected()) {
        _client.disconnect();
        Log.notice("Disconnected from MQTT broker." CR);
    } else {
        Log.warning("Tried to disconnect, but not connected to MQTT broker." CR);
    }
}

void MqttClient::publish(const char* topic, const char* message) {
    if (_client.connected()) {
        _client.publish(topic, message);
        Log.notice("Published message to topic %s: %s" CR, topic, message);
    } else {
        Log.error("Failed to publish message. Not connected to MQTT broker." CR);
    }
}

void MqttClient::subscribe(const char* topic) {
    if (_client.connected()) {
        _client.subscribe(topic);
        Log.notice("Subscribed to topic: %s" CR, topic);
    } else {
        Log.error("Failed to subscribe. Not connected to MQTT broker." CR);
    }
}

bool MqttClient::isConnected() {
    return _client.connected();
}

void MqttClient::loop() {
    if (_client.connected()) {
        _client.loop();
    }
}
