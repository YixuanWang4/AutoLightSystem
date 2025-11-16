/*
This file is for the servo command module of the Auto Light System project.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
Maintainer: 
Finish your code here.
*/

#include <servoCmd.h>

//Function to initialize Servo module and register servo pin before use, return true if successful
bool servoInit() {
    return true;
}

/**
 * Function to set the servos' power by swutching on BJT, used when servos need to take actions
 * 
 * @param powerMode true for power on, false for power off
 * @return true if successful, false otherwise
 */
bool servoSetPower(bool powerMode) {
    return true;
}

/**
 * Function to set the servo angle via PWM signal
 * @param servoNumber The servo number to set
 * @param mode The action servo should take(e.g. switch on/off).1 for ON, -1 for OFF
 */
void servoSetMode(int servoNumber, int mode) {
    
}