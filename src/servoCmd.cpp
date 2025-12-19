/*
This file is for the servo command module of the Auto Light System project.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
Maintainer: 
Finish your code here.
*/
#include <Arduino.h>
#include <servoCmd.h>
#include <ESP32Servo.h>
const int kMaxServoNum = 3;  // Maximum number of servos supported
const int kBjtControlPin = 12;  // Example BJT control pin
const int kServoOnAngle1 = 130;
const int kServoOnAngle2 = 50; // Angle for servo ON position
const int kServoOffAngle = 90;  // Angle for servo OFF position
bool servoInitialized = false;  // Flag to check if servo module is initialized
int servoPins[3] = {7, 10, 2};
Servo servoarr[kMaxServoNum];  // Array to hold servo objects
//Function to initialize Servo module and register servo pin before use, return true if successful
bool servoInit() {
    pinMode(kBjtControlPin, OUTPUT);
    digitalWrite(kBjtControlPin, HIGH);
    for (int i = 0; i < kMaxServoNum; ++i) {
        const int signal_pin = servoPins[i];
        if (signal_pin < 0 || signal_pin > 13) {
            return false;
        }
        servoarr[i].attach(signal_pin,1000,2000);
        vTaskDelay(500);
        Serial.print("Servo ");
        Serial.print(i);
        Serial.println(" initialized.");
    }
    digitalWrite(kBjtControlPin, LOW);
    servoInitialized = true;
    return true;
}

/**
 * Function to set the servos' power by switching on BJT, used when servos need to take actions
 * 
 * @param powerMode true for power on, false for power off
 * @return true if successful, false otherwise
 */
bool servoSetPower(bool powerMode) {
    if (!servoInitialized) {
        return false;  // Servo module not initialized
    }
    if (powerMode) {
        digitalWrite(kBjtControlPin, HIGH);  // Power on BJT
    } else {
        digitalWrite(kBjtControlPin, LOW);  // Power off BJT
    }
    return true;
}

/**
 * Function to set the servo angle via PWM signal
 * @param servoNumber The servo number to set
 * @param mode The action servo should take(e.g. switch on/off).1 for ON, -1 for OFF
 */
void servoSetMode(int servoNumber, int mode) {
    if (!servoInitialized || servoNumber < 0 || servoNumber >= kMaxServoNum) {
        return;  // Invalid servo number or not initialized
    }
    if (!servoSetPower(true)) {
        return;  // Failed to set power
    }
    int targetAngleGo = kServoOffAngle;
    int targetAngleBack = kServoOffAngle;
    switch (mode) {
        case 1:
            targetAngleGo = kServoOnAngle1;
            targetAngleBack = kServoOnAngle2;
            break;
        case -1:
            targetAngleGo = kServoOnAngle2;
            targetAngleBack = kServoOnAngle1;
            break;
        default:
            servoSetPower(false);  // Invalid mode, turn off power
            return;  // Invalid mode, do nothing
    }
    servoarr[servoNumber].write(targetAngleGo);  // Set the servo to the target angle
    delay(500);  // Wait for servo to reach position
    servoarr[servoNumber].write(targetAngleBack);  // Set to OFF position after action
    delay(500);  // Wait for servo to reach position
    servoarr[servoNumber].write(kServoOffAngle);
    servoSetPower(false);  // Turn off power after action
    return;
}