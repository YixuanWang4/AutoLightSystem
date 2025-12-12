/*
This file is for the servo command module of the Auto Light System project.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
Maintainer: 
Finish your code here.
*/
#include <Arduino.h>
#include <servoCmd.h>
<<<<<<< HEAD

//Function to initialize Servo module and register servo pin before use, return true if successful
bool servoInit() {
=======
#include <ESP32Servo.h>
const int kMaxServoNum = 3;  // Maximum number of servos supported
const int kServoPin[kMaxServoNum] = {5, 6, 9};  // Example servo pins
const int kBjtControlPin = 12;  // Example BJT control pin
const int kServoOnAngle1 = 0;
const int kServoOnAngle2 = 180; // Angle for servo ON position
const int kServoOffAngle = 90;  // Angle for servo OFF position
bool servoInitialized = false;  // Flag to check if servo module is initialized 
Servo servoarr[kMaxServoNum];  // Array to hold servo objects
//Function to initialize Servo module and register servo pin before use, return true if successful
bool servoInit() {
    pinMode(kBjtControlPin, OUTPUT);
    digitalWrite(kBjtControlPin, LOW);  // Ensure BJT is off initially
    for (int i = 0; i < kMaxServoNum; ++i) {
        const int signal_pin = kServoPin[i];
        if (signal_pin < 0 || signal_pin > 13) {
        return false;
        }
       servoarr[i].attach(signal_pin,1000,2000);
       servoarr[i].write(kServoOffAngle);  // Set initial position to OFF
    }
    servoInitialized = true;
>>>>>>> servo
    return true;
}

/**
 * Function to set the servos' power by swutching on BJT, used when servos need to take actions
 * 
 * @param powerMode true for power on, false for power off
 * @return true if successful, false otherwise
 */
bool servoSetPower(bool powerMode) {
<<<<<<< HEAD
=======
    if (!servoInitialized) {
        return false;  // Servo module not initialized
    }
    if (powerMode) {
        digitalWrite(kBjtControlPin, HIGH);  // Power on BJT
    } else {
        digitalWrite(kBjtControlPin, LOW);  // Power off BJT
    }
>>>>>>> servo
    return true;
}

/**
 * Function to set the servo angle via PWM signal
 * @param servoNumber The servo number to set
 * @param mode The action servo should take(e.g. switch on/off).1 for ON, -1 for OFF
 */
void servoSetMode(int servoNumber, int mode) {
<<<<<<< HEAD
    
=======
    if (!servoInitialized || servoNumber < 0 || servoNumber >= kMaxServoNum) {
        return;  // Invalid servo number or not initialized
    }
    if (!servoSetPower(true)) {
        return;  // Failed to set power
    }
    int targetAngle = kServoOffAngle;  // Default to OFF position
    switch (mode) {
        case 1:  // ON position
            targetAngle = kServoOnAngle1;  // Set to ON angle for servo 1
            break;
        case -1:  // ON position for servo 2
            targetAngle = kServoOnAngle2;  // Set to ON angle for servo 2
            break;
        default:
            servoSetPower(false);  // Invalid mode, turn off power
            return;  // Invalid mode, do nothing
    }
    servoarr[servoNumber].write(targetAngle);  // Set the servo to the target angle
    delay(1000);  // Wait for servo to reach position
    servoarr[servoNumber].write(kServoOffAngle);  // Set to OFF position after action
    delay(1000);  // Wait for servo to reach position
    servoSetPower(false);  // Turn off power after action
    return;
>>>>>>> servo
}