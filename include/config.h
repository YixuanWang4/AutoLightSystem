/*
This is the header file for the Auto Light System project, used for storing constant and configuration.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
If you have any varaible or constant that you want to use across multiple files, put it here.
*/

#ifndef _config_h_
#define _config_h_

#include <Arduino.h>

#define WIFI_CONN (1 << 0)

#define WIFISSID "AutoLightSys" // Replace with your WiFi SSID
#define WIFIPWD "111114115" // Replace with your WiFi password
#define THINGS_CLOUD_MQTT_HOST "mqtt://sh-5-mqtt.iot-api.com:1883"
#define THINGS_CLOUD_MQTT_ACCESSTOKEN "i5izm1ibp5bynpna"
#define THINGS_CLOUD_MQTT_PROJECTKEY  "CgoH8f6dcJ"

int servoPins[3] = {7, 6, 10};

const TickType_t wifiReconnPeriod = 20000; // Period to attempt WiFi reconnection in milliseconds

#endif