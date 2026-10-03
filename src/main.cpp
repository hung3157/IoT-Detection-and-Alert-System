#include <Arduino.h>
#include <WiFi.h>

unsigned long previousScan = 0;

constexpr float RSSI_AT_1M = -45.0;
constexpr float PATH_LOSS_N = 2.4;

float estimateDistance(int rssi) {
    float exponent =
        (RSSI_AT_1M - rssi) /
        (10.0 * PATH_LOSS_N);

    return pow(10.0, exponent);
}

void scanWiFi() {
    Serial.println();
    Serial.println("=== SCANNING ===");

    int n = WiFi.scanNetworks();

    for (int i = 0; i < n; i++) {
        Serial.printf(
            "%d. %s | RSSI %d dBm | CH %d\n",
            i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i), WiFi.channel(i)
        );
        Serial.printf(
            "   Estimated distance: %.2f meters\n",
            estimateDistance(WiFi.RSSI(i))
        );

    }

    WiFi.scanDelete();
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    Serial.println("IoT Sentinel started");
}

void loop() {
    unsigned long now = millis();

    if (now - previousScan >= 5000) {
        previousScan = now;

        scanWiFi();
    }
    
}