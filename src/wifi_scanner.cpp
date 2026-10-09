#include "wifi_scanner.h"

WiFiScanner::WiFiScanner(unsigned long timeDiff){
    scanIntervalMs_ = timeDiff;
    lastScanMs_ = 0;
    observationCount_ = 0;
    for (size_t i = 0; i < kMaxObservations; ++i) {
        observations_[i] = RadioObservation{};
    }
}

void WiFiScanner::begin() {
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    WiFi.disconnect();
    delay(100);
}

String WiFiScanner::formatMac(const uint8_t mac[6]) {
    char buffer[18];
    snprintf(buffer, sizeof(buffer), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(buffer);
}

void WiFiScanner::printObservation(const RadioObservation& observation) {
    Serial.printf("BSSID: %s|", formatMac(observation.mac).c_str());

    if (observation.hasSSID && observation.ssid.length() > 0) {
        Serial.printf("SSID: %s|", observation.ssid.c_str());
    } else {
        Serial.println("SSID: hidden or unavailable|");
    }

    Serial.printf("RSSI: %d dBm|", observation.rssi);
    Serial.printf("CH: %d|", observation.channel);
    Serial.printf("TS: %lu ms\n", observation.timestamp);
}

bool WiFiScanner::scanOnce() {
    observationCount_ = 0;

    int foundNetworks = WiFi.scanNetworks();
    if (foundNetworks < 0) {
        Serial.println("WiFi scan failed");
        return 0;
    }

    for (int i = 0; i < foundNetworks && i < kMaxObservations; ++i) {

        RadioObservation observation{};

        memcpy(observation.mac, WiFi.BSSID(i), 6);
        
        observation.rssi = WiFi.RSSI(i);
        observation.channel = WiFi.channel(i);
        observation.timestamp = millis();

        const String ssid = WiFi.SSID(i);
        observation.hasSSID = !ssid.isEmpty();
        observation.ssid = ssid;

        observations_[observationCount_++] = observation;

    }

    WiFi.scanDelete();
    return observationCount_;
}

/*
bool WiFiScanner::update() {
    unsigned long now = millis();
    if ((now - lastScanMs_) >= scanIntervalMs_) {
        lastScanMs_ = now;
        return scanOnce();
    }
    return 0;
}
*/
const RadioObservation* WiFiScanner::observations() const {
    return observations_;
}

size_t WiFiScanner::observationCount() const {
    return observationCount_;
}
