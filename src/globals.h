#ifndef GLOBALS_H
#define GLOBALS_H

#include <WiFi.h>

// Wi-Fi credentials
extern const char* ssid;
extern const char* password;
extern const char* mqttUserName;
extern const char* mqttPwd;
extern const char* clientID;
extern const char* SECRET_OTAPwd;

// LEDs
extern const int wifiLed;
extern const int mqttLed;
extern const int statusLed;
extern const int configLed;
extern const int actLed;

// Wi-Fi and MQTT clients
extern WiFiClient espClient;

#endif // GLOBALS_H
