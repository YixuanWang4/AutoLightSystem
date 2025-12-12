/*
This file is for the WiFi command module of the Auto Light System project.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
Maintainer: 
Finish your code here.
*/

#include <WiFiCmd.h>
#include <ThingsCloudMQTT.h>
#include <ThingsCloudWiFiManager.h>
#include <ArduinoJson.h>
#include <servoCmd.h>

// ThingsCloud SDK client instance (pointer so we can construct at runtime)
static ThingsCloudMQTT *tcClient = nullptr;
static TaskHandle_t tcMaintainTaskHandle = NULL;

// Fixed-size arrays for servo state cache. Known there are 3 servos.
#define MAX_SERVOS 3
// Indexing helper: accept 1..MAX_SERVOS or 0..(MAX_SERVOS-1)
static inline int servoIndex(int servoNumber) {
    if (servoNumber >= 1 && servoNumber <= MAX_SERVOS) return servoNumber - 1;
    if (servoNumber >= 0 && servoNumber < MAX_SERVOS) return servoNumber;
    return -1;
}

// 0 = off/default, >0 = mode id
static int servoDesiredMode[MAX_SERVOS] = {0, 0, 0};
static int servoAppliedMode[MAX_SERVOS] = {0, 0, 0};

// Helper: process an attributes JsonObject from ThingsCloud and apply to servos
void handleAttributes(const JsonObject &obj) {
    for (JsonPair kv : obj) {
        const char *key = kv.key().c_str();
        // Expect attribute keys like "servo1", "servo2", "servo3" or "servo_<n>"
        if (strncmp(key, "servo", 5) == 0) {
            const char *suffix = key + 5; // points to the number part
            int num = atoi(suffix);
            int idx = servoIndex(num);
            if (idx < 0) {
                Serial.printf("handleAttributes: invalid servo attribute key '%s'\n", key);
                continue;
            }
            int mode = 0;
            // value may be bool or number; try integer conversion
            if (kv.value().is<int>()) {
                mode = kv.value().as<int>();
            } else if (kv.value().is<bool>()) {
                mode = kv.value().as<bool>() ? 1 : 0;
            } else if (kv.value().is<const char*>()) {
                mode = atoi(kv.value().as<const char*>());
            }

            Serial.printf("handleAttributes: set servo %d mode=%d from attribute '%s'\n", num, mode, key);
            servoDesiredMode[idx] = mode;
            // apply to hardware
            servoSetMode(num, mode);
            servoAppliedMode[idx] = mode;
        }
    }
}

// This function MUST be implemented by the sketch per ThingsCloudMQTT.h
// It's called once the device is connected to ThingsCloud MQTT broker.
void onMQTTConnect() {
    Serial.println("ThingsCloud: onMQTTConnect called");
    if (tcClient == nullptr) return;

    // Subscribe to attribute-get responses (used after we call getAttributes())
    tcClient->onAttributesGetResponse([](const String &topic, const JsonObject &obj) {
        // SDK returns a result wrapper; check if result==1 then process attributes
        if (obj.containsKey("result") && obj["result"].as<int>() == 1 && obj.containsKey("attributes")) {
            handleAttributes(obj["attributes"].as<JsonObject>());
        }
    });

    // Subscribe to cloud-initiated attribute pushes
    tcClient->onAttributesPush([](const JsonObject &obj) {
        handleAttributes(obj);
    });

    // Request current attributes from cloud to initialize servo states
    tcClient->getAttributes();
}

// Background task: periodically call SDK loop() to maintain connections and process messages
static void tcMaintainTask(void *parameter) {
    while (true) {
        if (tcClient) {
            tcClient->loop();
        }
        vTaskDelay(1000);
    }
    vTaskDelete(NULL);
}

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
    // 1) Ensure WiFi is connected before attempting ThingsCloud connection
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Cannot connect to ThingsCloud: WiFi is not connected");
        return false;
    }

    // 2) Instantiate ThingsCloudMQTT client if not already created
    if (tcClient == nullptr) {
        Serial.println("Creating ThingsCloudMQTT client...");
        // Use constructor that accepts deviceKey, projectKey, typeKey and apiEndpoint
        tcClient = new ThingsCloudMQTT(THINGSCLOUD_HOST, String(THINGSCLOUD_DEVICE_KEY), THINGSCLOUD_PROJECT_KEY, String(THINGSCLOUD_TYPE_KEY), THINGSCLOUD_API_ENDPOINT);

        // Provide WiFi credentials to the SDK so it can manage WiFi if needed
        tcClient->setWifiCredentials(WIFISSID, WIFIPWD);

        // Optional: enable debugging messages from the SDK
        tcClient->enableDebuggingMessages(true);

        // Set callbacks for wifi/mqtt status if needed
        tcClient->setOnMQTTConnect([](){ onMQTTConnect(); });

        // Initialize internal state (timers, tokens, etc.)
        tcClient->initState();

        // Start background task to call loop() periodically
        if (tcMaintainTaskHandle == NULL) {
            xTaskCreate(tcMaintainTask, "TC Maintain", 4096, NULL, 1, &tcMaintainTaskHandle);
        }
    }

    // 3) Wait a short while for the SDK to establish connections (it will manage retries internally)
    const int maxChecks = 10;
    int checks = 0;
    while (!tcClient->isConnected() && checks < maxChecks) {
        Serial.print("Waiting for ThingsCloud connection... (attempt ");
        Serial.print(checks + 1);
        Serial.println(")");
        // Let SDK do work in its loop (we also call loop in background task)
        tcClient->loop();
        vTaskDelay(1000);
        checks++;
    }

    if (tcClient->isConnected()) {
        Serial.println("Connected to ThingsCloud via ThingsCloud_ESP_SDK!");
        return true;
    }

    Serial.println("Failed to connect to ThingsCloud after waiting period");
    return false;
}

/**
 * Function to periodically check the specific servo status via WiFi connection, called in main loop
 * @param servoNumber The servo number to be checked
 * @return Return 1 if the servo needs to be turned on, return -1 if it needs to be turned off, return 0 if no action is needed
 */
int wifiCheckServoStatus(int servoNumber) {
    // If WiFi or IoT platform not connected, no action
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("wifiCheckServoStatus: WiFi not connected");
        return 0;
    }

    if (tcClient == nullptr || !tcClient->isConnected()) {
        Serial.println("wifiCheckServoStatus: ThingsCloud client not connected");
        return 0;
    }

    // Map servoNumber to array index and bounds-check
    int idx = servoIndex(servoNumber);
    if (idx < 0) {
        Serial.printf("wifiCheckServoStatus: invalid servo number %d\n", servoNumber);
        return 0;
    }

    int desired = servoDesiredMode[idx];
    int applied = servoAppliedMode[idx];

    // Decide if action is needed
    if (desired == applied) {
        Serial.printf("wifiCheckServoStatus: servo %d no action needed (mode=%d)\n", servoNumber, desired);
        return 0;
    }

    // If desired > applied, treat as "turn on" action; if desired < applied treat as "turn off".
    // This is a heuristic since we don't have separate hardware state reading here.
    if (desired > applied) {
        Serial.printf("wifiCheckServoStatus: servo %d needs TURN ON (desired=%d applied=%d)\n", servoNumber, desired, applied);
        return 1;
    } else {
        Serial.printf("wifiCheckServoStatus: servo %d needs TURN OFF (desired=%d applied=%d)\n", servoNumber, desired, applied);
        return -1;
    }
}

/**
 * Function to logout from the IOT platform, used when the system is going to deep sleep
 * @return true if disconnect to IoT platform is successful, false otherwise
 */
void wifiLogoutIOTPlatform() {
    // Cleanly stop ThingsCloud SDK and free resources
    if (tcClient) {
        Serial.println("Stopping ThingsCloud client...");
        delete tcClient;
        tcClient = nullptr;
    }

    // Stop background task if running
    if (tcMaintainTaskHandle != NULL) {
        vTaskDelete(tcMaintainTaskHandle);
        tcMaintainTaskHandle = NULL;
    }
}