#include <Arduino.h>

#include "wifi/wifi.h"
#include "mqtt/MqttClient.h"

// Déclaration des informations Wi-Fi
const char *ssid = "TrainNetwork";
const char *password = "TrainNetwork";
const char *mqtt_server = "192.168.137.1";
const char *mqtt_user = "mqtt_user";
const char *mqtt_password = "mqtt_password";




// Initialisation du Wi-Fi et MQTT
WifiManager wifiManager(ssid, password);
MqttClient mqttClient(mqtt_server, 1883, mqtt_user, mqtt_password);

// Initialisation du module relais


///// TEMP /////

#define pubbksr "trains/track/relay/"

///// TEMP /////






void setup() {
    delay(1000);

    Serial.begin(115200);
    Log.begin(LOG_LEVEL_NOTICE, &Serial);

    Log.notice(F("-------------------------\n"));
    // Connexion au Wi-Fi
    wifiManager.connect();

    // Connexion au serveur MQTT
    mqttClient.connect();

    // Vérifier la connexion MQTT
    if (mqttClient.isConnected()) {
        Serial.println("MQTT client connected. Initializing relays...");

        // Initialiser les relais


        Serial.println("Relays initialized.");
    } else {
        Serial.println("MQTT client not connected at setup.");
    }
    Log.notice(F("-------------------------\n"));
}

void loop() {
    mqttClient.loop();

    static unsigned long lastDisplayTime = 0;
    if (millis() - lastDisplayTime > 5000) {
        if (wifiManager.isConnected() && mqttClient.isConnected()) {
        }

        lastDisplayTime = millis();
    }

    delay(100);
}