# RailWay_MQTT_Relay

Firmware ESP32-WROOM-32E pour piloter des relais via MQTT.

## Prérequis
- Broker MQTT accessible
- WiFi SSID / Password
- PlatformIO + dépendances (`PubSubClient`, `ArduinoLog`)

## Installation
```bash
git clone https://github.com/Regu01/RailWay_MQTT_Relay.git
cd RailWay_MQTT_Relay
pio run
pio run --target upload
