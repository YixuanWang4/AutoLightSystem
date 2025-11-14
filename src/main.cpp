/*
  This is the main.cpp file for the project, which is the assignment of the Engineering Course.
  It is written in C++11 standard, using Arduino framework and Google Coding Style.
  The project will be running via the FreeRTOS (an operating system).
  Maintainer: Yixuan Wang.
*/

#include <main.h>

void setup() {
  // put your setup code here, to run once:

  //Initialize the system
  bool isInit = systemInit();
  if(isInit) {
    Serial.println("System Initialization Successful!");
  }else{
    Serial.println("System Initialization Failed!");
    while(true){
      vTaskDelay(1000);
      Serial.print('.');
    }// Halt the system if initialization fails
  }

  //Join WiFi network.
  wifiScan();
  bool wifiLoginStatus = wifiLogin();
  if(wifiLoginStatus) {
    Serial.println("Logged in to WiFi Successfully!");
    xEventGroupSetBits(wifiEventGroup, WIFI_CONN);
  }else{
    Serial.println("WiFi Login Failed!");
    xEventGroupClearBits(wifiEventGroup, WIFI_CONN);
  }

  //Establish BLE Server.
  //Set BLE parameters and register callbacks via btSetUp().
  btSetUp();
  xTaskCreate(btMaintain, "BT Maintain Task", 4096, NULL, 1, NULL);



}

void loop() {
  // put your main code here, to run repeatedly:
}

//Function to initialize the system first
bool systemInit() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);

  // initialization for WiFi, BT, servo modules
  bool wifiStatus = wifiInit();
  if(!wifiStatus) {
    Serial.println("WiFi Initialization Failed!");
    return false;
  }

  bool btStatus = btInit();
  if(!btStatus) {
    Serial.println("Bluetooth Initialization Failed!");
    return false;
  }

  bool servoStatus = servoInit();
  if(!servoStatus) {
    Serial.println("Servo Initialization Failed!");
    return false;
  }

  // Initialize FreeRTOS components
  wifiEventGroup = xEventGroupCreate();
  if(wifiEventGroup == NULL){
    Serial.println("Event Group Creation Failed!");
    return false;
  }
  
  return true;
}


//This task will keep logging in WiFi if disconnected
void keepLoginWiFi(void * parameters) {
  while(true) {
    // Check WiFi connection status
    if(WiFi.status() != WL_CONNECTED || (xEventGroupGetBits(wifiEventGroup) & WIFI_CONN) == 0){
      Serial.println("WiFi Disconnected! Attempting to Reconnect...");
      bool loginStatus = wifiLogin();
      if(loginStatus) {
        Serial.println("Reconnected to WiFi Successfully!");
        xEventGroupSetBits(wifiEventGroup, WIFI_CONN);
      }else{
        Serial.println("WiFi Reconnection Failed!");
        xEventGroupClearBits(wifiEventGroup, WIFI_CONN);
      }
    }
    // Delay for a period before checking again
    vTaskDelay(wifiReconnPeriod);
  }

  vTaskDelete(NULL); // Delete the task if it ever exits the loop
}