/*
This is the header file main.h for the Auto Light System project.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
We will put here the common includes, definitions, and function declaration for the project.
*/

#include <Arduino.h>

#include <nvs_flash.h>

#include <config.h>
#include <WiFiCmd.h>
#include <BTCmd.h>
#include <servoCmd.h>
#include <slp_batCmd.h>

bool systemInit();