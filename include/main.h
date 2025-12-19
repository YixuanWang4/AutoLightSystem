/*
This is the header file main.h for the Auto Light System project.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
We will put here the common includes, definitions, and function declaration for the project.
*/
#ifndef _main_h_
#define _main_h_

#include <Arduino.h>

#include <nvs_flash.h>
#include <nvs.h>

#include <config.h>
#include <WiFiCmd.h>
#include <BTCmd.h>
#include <servoCmd.h>
#include <slp_batCmd.h>

EventGroupHandle_t wifiEventGroup;
TaskHandle_t keepLoginWifiTaskHandle;
extern bool IOTState[3];
extern ThingsCloudMQTT client;

bool systemInit();

void keepLoginWiFi(void *parameters);


#endif
