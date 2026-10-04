#include <Arduino.h>

#include "wifi_scanner.h"

WiFiScanner wifiScanner(5000);

void setup() {
    Serial.begin(115200);
    delay(1000);
    wifiScanner.begin();
    Serial.println("IoT Sentinel started");
}

void loop() {
    if (wifiScanner.update()) {

        const auto* observations = wifiScanner.observations();
        const size_t count = wifiScanner.observationCount();


        if (count <= 0) {
            Serial.printf("No WiFi networks found\n");
        } 
        else {
            Serial.printf("=== WIFI SCAN ===\n");
        }

        for (size_t i = 0; i < count; ++i) {
            Serial.printf("[%zu]\n", i + 1);

            WiFiScanner::printObservation(observations[i]);

        }
    }
}
