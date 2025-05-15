#pragma once

// ----- Identifiants -----
#define DEVICE_ID       "RAILWAY_ESP_RELAY_01"


// ----- WiFi -----
#define WIFI_SSID      "Freebox-Peyon"
#define WIFI_PASSWORD  "Peyon17t*"

// ----- MQTT -----
#define MQTT_HOST      "192.168.1.167"
#define MQTT_PORT      1883
#define MQTT_USER      ""
#define MQTT_PASS      ""

// Base topic
#define BASE_TOPIC      "railway"

// ----- Relais -----
#define RELAY_COUNT     8
static const uint8_t RELAY_PINS[RELAY_COUNT] = {32, 33, 25, 26, 27, 14, 12, 13};

// ----- Paramètres -----
#define WIFI_MAX_RETRIES     10
#define MQTT_BACKOFF_INITIAL 5    // secondes
#define MQTT_BACKOFF_MAX     60   // secondes
#define METRICS_INTERVAL_MS  10000