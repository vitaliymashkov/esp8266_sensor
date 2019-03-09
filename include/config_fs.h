//
// Created by vitaliy on 06.03.19.
//

#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <ArduinoJson.h>
#include <FS.h>

struct Sensor {
    String type;
    String pin;
};

struct Config {
    String ssid;
    String password;
    String host;
    Sensor sensors[9];
};

void initFS() {
    if (!SPIFFS.begin())
    {
        // Serious problem
        Serial.println("SPIFFS Mount failed");
    } else {
        Serial.println("SPIFFS Mount succesfull");
    }
}

void configFromString(String &data, Config &config){
    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<512> doc;
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, data);
    if (error) {
        Serial.println(F("Failed to read file, using default configuration"));
    }
    serializeJsonPretty(doc, Serial);
    // Copy values from the JsonDocument to the Config
    if (doc["ssid"] != "") {
        const char *s = doc["ssid"];
        config.ssid = s;
    } else {
        Serial.println("ssid empty");
        config.ssid = "";
    }
    if (doc["password"] != "") {
        const char *p = doc["password"];
        config.password = p;
    } else {
        Serial.println("password empty");
        config.password = "";
    }
    if (doc["host"] != "") {
        const char *h = doc["host"];
        config.host = h;
    } else {
        Serial.println("host empty");
        config.host = "";
    }
}

// Loads the configuration from a file
bool loadConfiguration(const char *filename, Config &config) {
    if (SPIFFS.exists(filename)) {
        // Open file for reading
        File file = SPIFFS.open(filename, "r");

        String fileData = "";
        while (file.available()) {
            fileData = fileData + (char)file.read();
        }
        Serial.print("Config Content: ");
        Serial.println(fileData);
        configFromString(fileData, config);
        // Close the file (Curiously, File's destructor doesn't close the file)
        file.close();
        return true;
    } else {
        Serial.print("File not found: ");
        Serial.println(filename);
        return false;
    }
}

String configToString(const Config &config) {
    String output;
    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<256> doc;

    // Set the values in the document
    doc["ssid"] = config.ssid;
    doc["password"] = config.password;
    doc["host"] = config.host;
    serializeJson(doc, output);
    return output;
}

// Saves the configuration to a file
void saveConfiguration(const char *filename, const Config &config) {
    // Delete existing file, otherwise the configuration is appended to the file
    SPIFFS.remove(filename);

    // Open file for writing
    File file = SPIFFS.open(filename, "w+");
    if (!file) {
        Serial.println(F("Failed to create file"));
        return;
    }
    // Serialize JSON to file
    file.print(configToString(config));

    // Close the file
    file.close();
}

// Prints the content of a file to the Serial
void printFile(const char *filename) {
    // Open file for reading
    File file = SPIFFS.open(filename, "r");
    if (!file) {
        Serial.println(F("Failed to read file"));
        return;
    }

    // Extract each characters by one by one
    while (file.available()) {
        Serial.print((char)file.read());
    }
    Serial.println();

    // Close the file
    file.close();
}


#endif //__CONFIG_H__
