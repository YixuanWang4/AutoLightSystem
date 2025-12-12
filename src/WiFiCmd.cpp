/*
This file is for the WiFi command module of the Auto Light System project.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
Maintainer: 
Finish your code here.
*/

#include <WiFiCmd.h>
#include <config.h>

ThingsCloudMQTT client(
  THINGS_CLOUD_MQTT_HOST,
  THINGS_CLOUD_MQTT_ACCESSTOKEN,
  THINGS_CLOUD_MQTT_PROJECTKEY);

bool currentState[3] = {false, false, false};


//Function to initialize WiFi module before connecting to the network, return true if successful
bool wifiInit() {
    WiFi.mode(WIFI_STA);
    return true;
}

//Function to log in to the WiFi network, return true if successful
bool wifiLogin() {
    int cnt = 0;
    Serial.println("Connecting to WiFi...");
    client.setWifiCredentials(WIFISSID, WIFIPWD);

    while(!client.isWifiConnected()) {
        vTaskDelay(500);
        cnt++;
        if(cnt >= 20) { // Timeout after 10 seconds
            Serial.println("WiFi Connection Timeout!");
            return false;
        }
    }
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

bool checkWifiConnIOTPlatform() {
    client.enableDebuggingMessages();
    if(!client.isMqttConnected()){
        Serial.println("MQTT Connection Failed!");
        return false;
    }
    return true;
}

void onMQTTConnect() {
    Serial.println("Connected to ThingsCloud!");
    client.onAttributesGetResponse([](const String &topic, const JsonObject &obj) {
        if (obj["result"] == 1) {
            handleAttributes(obj["attributes"]);
        }
    });
    client.onAttributesPush([](const JsonObject &obj) {
        handleAttributes(obj);
    });
    client.getAttributes();
}

<<<<<<< HEAD
/**
 * Function to logout from the IOT platform, used when the system is going to deep sleep
 * @return true if disconnect to IoT platform is successful, false otherwise
 */
void wifiLogoutIOTPlatform() {
    
=======
void handleAttributes(const JsonObject &obj) {
    Serial.println("Received attributes update from ThingsCloud");   
        if (obj.containsKey( "frontLightState")) {
            currentState[0] = obj[ "frontLightState"];
        }
        if (obj.containsKey("backLightState")) {
            currentState[1] = obj["backLightState"];
        }
        if (obj.containsKey("washLightState")) {
            currentState[2] = obj["washLightState"];
        }
    }


//Function to initialize servos
void initServos() {
    for (int i = 0; i < 3; i++) {
        pinMode(servoPins[i], OUTPUT);
        digitalWrite(servoPins[i], LOW);  
        Serial.print("Servo ");
        Serial.print(i + 1);
        Serial.println(" initialized");
    }
>>>>>>> wifi
}