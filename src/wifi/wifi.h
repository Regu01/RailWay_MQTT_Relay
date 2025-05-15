#ifndef WIFI_H
#define WIFI_H

#include <WiFi.h>
#include <ArduinoLog.h>

class WifiManager {
public:
    WifiManager(const char* ssid, const char* password);
    void connect();
    bool isConnected();
    void disconnect();
    void printStatus();

private:
    const char* _ssid;
    const char* _password;
    void logConnectionDetails();
};

#endif // WIFI_H
