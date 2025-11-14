/*
This is the header file WiFiCmd.h for the Auto Light System project.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
We will put here the common includes, definitions, and function declaration for the project.
*/

#ifndef _WiFiCmd_h_
#define _WiFiCmd_h_

#include <WiFi.h>

#include <config.h>

bool wifiInit();

bool wifiLogin();

void wifiScan();

#endif