/*
This is the header file WiFiCmd.h for the Auto Light System project.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
We will put here the common includes, definitions, and function declaration for the project.
*/

#ifndef _WiFiCmd_h_
#define _WiFiCmd_h_

#include <WiFi.h>

#include <config.h>

#include <ThingsCloudWiFiManager.h>

#include <ThingsCloudMQTT.h>


bool wifiInit();

bool wifiLogin();

void wifiScan();

bool checkWifiConnIOTPlatform() ;

void onMQTTConnect() ;

void handleAttributes(const JsonObject &obj) ;

void initServos() ;

#endif