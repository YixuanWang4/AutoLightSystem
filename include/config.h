/*
This is the header file for the Auto Light System project, used for storing constant and configuration.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
If you have any varaible or constant that you want to use across multiple files, put it here.
*/

#ifndef _config_h_
#define _config_h_

#include <Arduino.h>

#define WIFI_CONN (1 << 0)


#define WIFISSID "SJTU" // Replace with your WiFi SSID
#define WIFIPWD "Joe20061001*" // Replace with your WiFi password

const TickType_t wifiReconnPeriod = 20000; // Period to attempt WiFi reconnection in milliseconds

#endif