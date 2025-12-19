/*
This is the header file for the Auto Light System project, used for storing constant and configuration.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
If you have any varaible or constant that you want to use across multiple files, put it here.
*/

#ifndef _config_h_
#define _config_h_

#include <Arduino.h>

#define WIFI_CONN (1 << 0)

#define WIFISSID "Mate 70 pro" // Replace with your WiFi SSID
#define WIFIPWD "71180600" // Replace with your WiFi password
#define THINGS_CLOUD_MQTT_HOST "sh-5-mqtt.iot-api.com"
#define THINGS_CLOUD_MQTT_ACCESSTOKEN "i5izm1ibp5bynpna"
#define THINGS_CLOUD_MQTT_PROJECTKEY  "CgoH8f6dcJ"

const TickType_t wifiReconnPeriod = 20000; // Period to attempt WiFi reconnection in milliseconds

#endif