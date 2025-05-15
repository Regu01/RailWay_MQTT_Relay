#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoLog.h>
#include <ArduinoJson.h>
#include "config.h"

WiFiClient   espClient;
PubSubClient mqtt(espClient);
int          retryCount = 0;
int          mqttBackoff = MQTT_BACKOFF_INITIAL;

void setupWiFi();
bool mqttReconnect();
void callback(char* topic, byte* payload, unsigned int len);
void publishMetrics();
void publishRelayStates();
void setupLogging();

void setup() {
  Serial.begin(115200);
  setupLogging();
  Log.info("Starting %s...\n", DEVICE_ID);

  // Initialize all relays off
  for (int i = 0; i < RELAY_COUNT; ++i) {
    pinMode(RELAY_PINS[i], OUTPUT);
    digitalWrite(RELAY_PINS[i], LOW);
  }

  // Connect to WiFi
  setupWiFi();

  // MQTT configuration
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(callback);

  // Initial MQTT connect
  if (mqttReconnect()) {
    publishRelayStates();
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    setupWiFi();
  }

  if (!mqtt.connected()) {
    if (mqttReconnect()) {
      publishRelayStates();
    } else {
      retryCount++;
    }
  }

  mqtt.loop();

  static unsigned long lastMs = 0;
  if (millis() - lastMs > METRICS_INTERVAL_MS) {
    lastMs = millis();
    publishMetrics();
  }
}

void setupWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts++ < WIFI_MAX_RETRIES) {
    delay(500);
    Log.warning("WiFi try #%d...\n", attempts);
  }
  if (WiFi.status() == WL_CONNECTED) {
    Log.info("WiFi IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Log.error("WiFi failed after %d tries\n", WIFI_MAX_RETRIES);
  }
}

bool mqttReconnect() {
  String clientId    = String(DEVICE_ID);
  String topicStatus = String(BASE_TOPIC) + "/" + DEVICE_ID + "/status";
  String relayBase   = String(BASE_TOPIC) + "/" + DEVICE_ID + "/relay/";

  bool ok = mqtt.connect(
    clientId.c_str(),
    MQTT_USER, MQTT_PASS,
    topicStatus.c_str(),
    1, true, "offline"
  );

  if (ok) {
    Log.info("MQTT connected\n");
    mqtt.publish(topicStatus.c_str(), "online", true);
    // Subscribe to relay control topics
    mqtt.subscribe((relayBase + "+/set").c_str());
    mqtt.subscribe((relayBase + "+/state").c_str());
    mqttBackoff = MQTT_BACKOFF_INITIAL;
    retryCount  = 0;
    return true;
  }

  Log.warning("MQTT failed rc=%d, retry in %ds\n", mqtt.state(), mqttBackoff);
  delay(mqttBackoff * 1000);
  mqttBackoff = min(mqttBackoff * 2, MQTT_BACKOFF_MAX);
  return false;
}

void callback(char* topic, byte* payload, unsigned int len) {
  String msg;
  for (unsigned int i = 0; i < len; ++i) {
    msg += (char)payload[i];
  }
  Log.notice("%s => %s\n", topic, msg.c_str());

  String prefix = String(BASE_TOPIC) + "/" + DEVICE_ID + "/relay/";
  if (!msg.isEmpty() && String(topic).startsWith(prefix)) {
    String suffix = String(topic).substring(prefix.length());
    int slash = suffix.indexOf('/');
    if (slash > 0) {
      int idx = suffix.substring(0, slash).toInt();
      String action = suffix.substring(slash + 1);
      if (idx >= 0 && idx < RELAY_COUNT && action == "set") {
        bool on = (msg == "ON");
        bool currentState = digitalRead(RELAY_PINS[idx]);
        if (currentState != on) {
          digitalWrite(RELAY_PINS[idx], on ? HIGH : LOW);
          String stateTopic = prefix + String(idx) + "/state";
          mqtt.publish(stateTopic.c_str(), on ? "ON" : "OFF", true);
        }
      }
    }
  }
}



void publishRelayStates() {
  String base = String(BASE_TOPIC) + "/" + DEVICE_ID + "/relay/";
  for (int i = 0; i < RELAY_COUNT; ++i) {
    String t = base + String(i) + "/state";
    const char* s = digitalRead(RELAY_PINS[i]) ? "ON" : "OFF";
    mqtt.publish(t.c_str(), s, true);
  }
}

void publishMetrics() {
  String metricsTopic = String(BASE_TOPIC) + "/" + DEVICE_ID + "/metrics";
  DynamicJsonDocument doc(256);
  doc["uptime_ms"]    = millis();
  doc["freeHeap"]     = ESP.getFreeHeap();
  doc["maxAllocHeap"] = ESP.getMaxAllocHeap();
  doc["rssi"]         = WiFi.RSSI();
  doc["mqtt_retries"] = retryCount;

  char buf[256];
  size_t n = serializeJson(doc, buf, sizeof(buf));
  mqtt.publish(metricsTopic.c_str(), (const uint8_t*)buf, n, false);
  Log.info("Metrics sent\n");
}

void setupLogging() {
  Log.begin(LOG_LEVEL_INFO, &Serial);
}