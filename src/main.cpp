#include <Arduino.h>

#include "wifi_scanner.h"

WiFiScanner wifiScanner(5000);

void setup() {
    Serial.begin(115200);
    delay(1000);
    wifiScanner.begin();
    Serial.println("\n**** IoT Sentinel started ****\n");
}

void loop() {
    if (wifiScanner.scanOnce()) {

        const auto* observations = wifiScanner.observations();
        const size_t count = wifiScanner.observationCount();
        Serial.printf("\n=== %d NETWORK FOUND ===\n", count);

        for (size_t i = 0; i < count; ++i) {
            Serial.printf("[%zu]\n", i + 1);

            WiFiScanner::printObservation(observations[i]);

        }
    }
    else { 
        Serial.printf("\n=== !!! NO NETWORK FOUND !!! ===\n");
    }
}

