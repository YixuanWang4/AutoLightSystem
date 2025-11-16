/*
This is the header file BTCmd.h for the Auto Light System project.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
We will put here the common includes, definitions, and function declaration for the project.
*/
#ifndef _BTCmd_h_
#define _BTCmd_h_

#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLESecurity.h>
#include <nvs_flash.h>


#define SERVICE_UUID BLEUUID((uint16_t)0x180D)
#define CHARACTERISTIC_UUID BLEUUID((uint16_t)0x2A37)
#define AUTH_PASSKEY 123456

bool btInit();

bool btSetUp();

void btMaintain(void *parameter);

#endif