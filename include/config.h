/*
This is the header file for the Auto Light System project, used for storing constant and configuration.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
If you have any varaible or constant that you want to use across multiple files, put it here.
*/

#ifndef _config_h_
#define _config_h_

#define WIFI_CONN (1 << 0)


const TickType_t wifiReconnPeriod = 5000; // Period to attempt WiFi reconnection in milliseconds

#endif