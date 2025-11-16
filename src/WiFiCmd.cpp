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

//Function to scan available WiFi networks and print them to Serial Monitor, used to assist developers to find out the correct SSID
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

/**
 * Function to connect to IoT platform after logging in to WiFi network, return true if successful
 * @return true if connection to IoT platform is successful, false otherwise
 */
bool wifiConnIOTPlatform() {
    return true;
}

/**
 * Function to periodically check the specific servo status via WiFi connection, called in main loop
 * @param servoNumber The servo number to be checked
 * @return Return 1 if the servo needs to be turned on, return -1 if it needs to be turned off, return 0 if no action is needed
 */
int wifiCheckServoStatus(int servoNumber) {

    return 0;
}

/**
 * Function to logout from the IOT platform, used when the system is going to deep sleep
 * @return true if disconnect to IoT platform is successful, false otherwise
 */
void wifiLogoutIOTPlatform() {

}