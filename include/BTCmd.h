/*
This is the header file BTCmd.h for the Auto Light System project.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
We will put here the common includes, definitions, and function declaration for the project.
*/
#ifndef _BTCmd_h_
#define _BTCmd_h_

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


#define SERVICE_UUID "70f1b487-05ae-43a2-99c7-d15d5ea114b6"
#define CHARACTERISTIC_UUID "54cb1d1e-556d-48e1-9bd2-3273e4921e2e"

bool btInit();

bool btSetUp();

void btMaintain(void *parameter);

#endif