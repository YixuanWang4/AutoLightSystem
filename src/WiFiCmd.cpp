/*
This file is for the WiFi command module of the Auto Light System project.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
Maintainer: 
Finish your code here.
*/

#include <WiFiCmd.h>

//Function to initialize WiFi module before connecting to the network, return true if successful
bool wifiInit() {
    WiFi.mode(WIFI_STA);
    return true;
}

//Function to log in to the WiFi network, return true if successful
bool wifiLogin() {
    int cnt = 0;
    Serial.println("Connecting to WiFi...");
    WiFi.setAutoReconnect(true);
    WiFi.begin(WIFISSID, WIFIPWD);
    while(WiFi.status() != WL_CONNECTED) {
        vTaskDelay(500);
        cnt++;
        if(cnt >= 20) { // Timeout after 10 seconds
            Serial.println("WiFi Connection Timeout!");
            return false;
        }
    }
    Serial.println("WiFi Connected!");
    Serial.print("IP Address: ");Serial.println(WiFi.localIP());
    return true;
}

void wifiScan() {
    Serial.println("Scanning available WiFi networks...");
    int n = WiFi.scanNetworks();
    if (n == 0) {
        Serial.println("No WiFi networks found!");
    } else {
        Serial.print(n);
        Serial.println(" networks found:");
        for (int i = 0; i < n; ++i) {

            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.println(")");
            delay(10);
        }
    }
}