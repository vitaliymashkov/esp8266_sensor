//
// Created by vitaliy on 07.03.19.
//

#ifndef ESP8266_ARDUINO_WIFI_INIT_H
#define ESP8266_ARDUINO_WIFI_INIT_H

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntp_primary);

const long intervalNtp = 1;
unsigned long previousMillisNtp = 0;

void iniWiFiAP(String ssid, String password) {
    Serial.print("Configuring access point...");
    WiFi.mode(WIFI_AP);
    /* You can remove the password parameter if you want the AP to be open. */
    WiFi.softAP(ssid, password);

    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void initWiFi(String ssid, String password) {
    Serial.println("Booting");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }

    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

unsigned long get_time() {
    unsigned long epochTime;
    unsigned long currentMillis = millis();
    String formattedDate;
    String formattedTime;

    if ((currentMillis - previousMillisNtp) / 1000 >= intervalNtp) {
        // Save the last time you read the sensor
        previousMillisNtp = currentMillis;

        if (timeClient.update()) {
            epochTime = timeClient.getEpochTime();
            formattedDate = timeClient.getFormattedDate();
            formattedTime = timeClient.getFormattedTime();
            Serial.println(formattedDate + " " + formattedTime + " :: " + String(epochTime));
        }
    }
    return epochTime;
}

#endif //ESP8266_ARDUINO_WIFI_INIT_H
