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

void initWiFi(String ssid, String password, IPAddress ip, IPAddress subnet, IPAddress gateway, String host, IPAddress dns1, IPAddress dns2) {
    Serial.println("Booting");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (ip.toString() != "0.0.0.0") {
        WiFi.config(ip, gateway, subnet, dns1, dns2);
    }
    if (host != "") {
        WiFi.hostname(host);
    }
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }
    WiFi.printDiag(Serial);
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Subnet: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("DNS1: ");
    Serial.println(WiFi.dnsIP(0));
    Serial.print("DNS2: ");
    Serial.println(WiFi.dnsIP(1));
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
}


void initWiFi(String ssid, String password, String host) {
    IPAddress e_ip(0,0,0,0);
    IPAddress e_subnet(0,0,0,0);
    IPAddress e_gateway(0,0,0,0);
    IPAddress e_dns1(0,0,0,0);
    IPAddress e_dns2(0,0,0,0);
    initWiFi(ssid, password, e_ip, e_subnet, e_gateway, host, e_dns1, e_dns2);
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
