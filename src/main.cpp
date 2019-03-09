
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <HTU21D.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ArduinoJson.h>

#include "config_fs.h"

#include "default_config.h"

#include "wifi_init.h"
#include "ota_init.h"

#define DHTTYPE DHT22   // DHT Shield uses DHT 11
#define DHTPIN D4       // DHT Shield uses pin D4

/*
HTU21D(resolution)
resolution:
HTU21D_RES_RH12_TEMP14 - RH: 12Bit, Temperature: 14Bit, by default
HTU21D_RES_RH8_TEMP12  - RH: 8Bit,  Temperature: 12Bit
HTU21D_RES_RH10_TEMP13 - RH: 10Bit, Temperature: 13Bit
HTU21D_RES_RH11_TEMP11 - RH: 11Bit, Temperature: 11Bit
*/
HTU21D myHTU21D(HTU21D_RES_RH12_TEMP14);

ESP8266WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);

float humidity, temperature;                 // Raw float values from the sensor
unsigned long epochTime;
char str_humidity[10], str_temperature[10];  // Rounded sensor values and as strings
// Generally, you should use "unsigned long" for variables that hold time
const long interval = 5;

unsigned long previousMillis = millis() + interval * 1000;

const char *filename = "/config.txt";  // <- SD library uses 8.3 filenames
Config config;                         // <- global configuration object


void initPins() {
    pinMode(A0, OUTPUT);
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
}

void read_sensor() {
    // Wait at least 2 seconds seconds between measurements.
    // If the difference between the current time and last time you read
    // the sensor is bigger than the interval you set, read the sensor.
    // Works better than delay for things happening elsewhere also.
    unsigned long currentMillis = millis();

    if ((currentMillis - previousMillis) / 1000 >= interval) {
        // Save the last time you read the sensor
        previousMillis = currentMillis;

        // Reading temperature and humidity takes about 250 milliseconds!
        // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
        //humidity = dht.readHumidity();        // Read humidity as a percent
        //temperature = dht.readTemperature();  // Read temperature as Celsius

        humidity = myHTU21D.readHumidity();        // Read humidity as a percent
        temperature = myHTU21D.readTemperature();  // Read temperature as Celsius

        // Check if any reads failed and exit early (to try again).
        if (isnan(humidity) || isnan(temperature)) {
            Serial.println("Failed to read from DHT sensor!");
            return;
        }

        // Convert the floats to strings and round to 2 decimal places
        dtostrf(humidity, 1, 2, str_humidity);
        dtostrf(temperature, 1, 2, str_temperature);

        Serial.print("Humidity: ");
        Serial.print(str_humidity);
        Serial.print(" %\t");
        Serial.print("Temperature: ");
        Serial.print(str_temperature);
        Serial.println(" °C");
    }
}




void sendData() {
    epochTime = get_time();
    StaticJsonDocument<500> doc;

    // Create the "analog" array
    JsonArray analogValues = doc.createNestedArray("analog");

    analogValues.add(analogRead(A0));

    // Create the "digital" array
    JsonObject digitalValues = doc.createNestedObject("digital");

    digitalValues["D0"] = digitalRead(D0);
    digitalValues["D1"] = digitalRead(D1);
    digitalValues["D2"] = digitalRead(D2);
    digitalValues["D3"] = digitalRead(D3);
    digitalValues["D4"] = digitalRead(D4);
    digitalValues["D5"] = digitalRead(D5);
    digitalValues["D6"] = digitalRead(D6);
    digitalValues["D7"] = digitalRead(D7);

    JsonObject mapValues = doc.createNestedObject("pin_map");

    mapValues["D0"] = (D0);
    mapValues["D1"] = (D1);
    mapValues["D2"] = (D2);
    mapValues["D3"] = (D3);
    mapValues["D4"] = (D4);
    mapValues["D5"] = (D5);
    mapValues["D6"] = (D6);
    mapValues["D7"] = (D7);

    doc["time"] = timeClient.getEpochTime();
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;

    Serial.print(F("Sending: "));
    serializeJsonPretty(doc, Serial);
    Serial.println();

    // Write response headers
    // server.sendHeader(F("HTTP/1.0 200 OK"));
    // server.sendHeader("Content-Type", "application/json");
    // server.sendHeader("Content-Length: ", String(measureJsonPretty(doc)));

    // Write JSON document
    String json;
    serializeJsonPretty(doc, json);

    server.sendHeader("Access-Control-Allow-Origin","*");
    server.send(200, "application/json", json);
}

void saveConfig() {
    server.sendHeader("Access-Control-Allow-Origin","*");
    if (server.hasArg("plain")== false){ //Check if body received
        server.send(200, "application/json", configToString(config));
        return;
    }
    String body = server.arg("plain");
    configFromString(body, config);
    saveConfiguration(filename, config);

    String message = "Body received:\n";
    message += body;
    message += "\nConfig saved:\n";
    message += configToString(config);
    message += "\n";

    server.send(200, "text/plain", message);
    Serial.println(message);
}

void setPinVal() {
    String message = "Number of args received:";
    message += server.args();            //Get number of parameters
    message += "\n";                            //Add a new line
    long pin = -1;
    long val;
    for (int i = 0; i < server.args(); i++) {
        message += "Arg #” + (String)i + ” –> ";
        message += server.argName(i) + ": ";
        message += server.arg(i) + "\n";
        if (server.argName(i) == "pin") {
            pin = server.arg(i).toInt();
        }
        if (server.argName(i) == "val") {
            val = server.arg(i).toInt();
        }
    }
    if (pin != -1) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, val);
    }
    Serial.println(message);
    Serial.print(pin);
    Serial.print(": ");
    Serial.println(val);
    sendData();
}

void initHttpServer() {
    // Route for root / web page
    server.serveStatic("/", SPIFFS, "/index.html");
    server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");
    // Handle http requests
    server.serveStatic("/css", SPIFFS, "/css");
    server.serveStatic("/img", SPIFFS, "/img");
    server.serveStatic("/js", SPIFFS, "/js");

    server.on("/data.json", sendData);
    server.on("/config", saveConfig);
    server.on("/set", setPinVal);

    // Start the web server
    server.begin();
    Serial.println("HTTP server started");
}


void setup() {
    initPins();
    Serial.begin(115200);

    initFS();
    if (loadConfiguration(filename, config)) {
        Serial.print("Host: ");
        Serial.println(config.host);
        Serial.print("Ssid: ");
        Serial.println(config.ssid);
        Serial.print("Password: ");
        Serial.println(config.password);
    }

    dht.begin();

    if (myHTU21D.begin() != true)
    {
        Serial.println(F("HTU21D, SHT21 sensor is faild or not connected")); //(F()) saves string to flash & keeps dynamic memory free
        delay(5000);
    }
    Serial.println(F("HTU21D, SHT21 sensor is active"));

    Serial.println("WeMos DHT Server");
    Serial.println("");
    if (config.ssid != "" && config.password != "") {
        Serial.print("DHCP mode: ");
        Serial.print(config.ip_mode);
        Serial.print(" isStatic ");
        Serial.print(config.ip_mode == "static");
        if (config.ip_mode == "static") {
            IPAddress ip, subnet, gateway, dns1, dns2;

            ip.fromString(config.ip);
            subnet.fromString(config.subnet);
            gateway.fromString(config.gateway);
            dns1.fromString(config.gateway);
            dns2.fromString(config.gateway);

            initWiFi(config.ssid, config.password, ip, subnet, gateway, config.host, dns1, dns2);
        } else {
            initWiFi(config.ssid, config.password, config.host);
        }
        initOTA(config.host);
    } else {
        iniWiFiAP(APSSID, APPSK);
        initOTA(STAHOST);
    }

    initHttpServer();

    timeClient.begin();
}

void loop() {
    ArduinoOTA.handle();
    server.handleClient();
    if (config.host != "") {
        read_sensor();
        epochTime = get_time();
    }
}
