#include "wifi.h"

WifiManager::WifiManager(const char* ssid, const char* password) 
    : _ssid(ssid), _password(password) {}

void WifiManager::connect() {
    Log.notice("Connecting to Wi-Fi: %s" CR, _ssid);
    WiFi.begin(_ssid, _password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10) {
        Log.notice("Attempt %d: Waiting for connection..." CR, attempts + 1);
        delay(1000);
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Log.notice("Connected to Wi-Fi successfully!" CR);
        logConnectionDetails();
    } else {
        Log.error("Failed to connect to Wi-Fi" CR);
    }
}

bool WifiManager::isConnected() {
    if (WiFi.status() == WL_CONNECTED) {
        return true;
    } else {
        Log.warning("Wi-Fi is not connected." CR);
        return false;
    }
}

void WifiManager::disconnect() {
    if (isConnected()) {
        WiFi.disconnect();
        Log.notice("Disconnected from Wi-Fi." CR);
    } else {
        Log.warning("Wi-Fi was not connected." CR);
    }
}

void WifiManager::printStatus() {
    if (isConnected()) {
        Log.notice("Connected to: %s with IP: %s" CR, _ssid, WiFi.localIP().toString().c_str());
    } else {
        Log.warning("Not connected to any Wi-Fi network." CR);
    }
}

void WifiManager::logConnectionDetails() {
    Log.notice("SSID: %s" CR, WiFi.SSID().c_str());
    Log.notice("Signal Strength: %d dBm" CR, WiFi.RSSI());
    Log.notice("IP Address: %s" CR, WiFi.localIP().toString().c_str());
}
