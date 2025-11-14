/*
This file is for the bluetooth command module of the Auto Light System project.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
Maintainer: 
Finish your code here.
*/

#include <BTCmd.h>

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
int connectedClients = 0;
bool deviceConnected = false;
uint32_t value = 0;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    connectedClients++;
    Serial.print("Client connected. Total clients: ");
    Serial.println(connectedClients);
    // Continue advertising for more connections
    BLEDevice::startAdvertising();
  };

  void onDisconnect(BLEServer *pServer) {
    connectedClients--;
    Serial.print("Client disconnected. Total clients: ");
    Serial.println(connectedClients);
  }
};

//Function to initialize Bluetooth module, return true if successful
bool btInit() {
    BLEDevice::init("AutoLightSys");
    return true;
}

bool btSetUp() {
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
    );
    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting for client connections to notify...");

    return true;
}

void btMaintain(void * parameter) {
  // Notify changed value to all connected clients
  while(true){
    if (connectedClients > 0) {
      Serial.print("Notifying value: ");
      Serial.print(value);
      Serial.print(" to ");
      Serial.print(connectedClients);
      Serial.println(" client(s)");
      pCharacteristic->setValue((uint8_t *)&value, 4);
      pCharacteristic->notify();
      value++;
      // Bluetooth stack will go into congestion, if too many packets are sent.
      // In 6 hours of testing, I was able to go as low as 3ms.
      // When using core debug level "debug" or "verbose", the delay can be increased in
      // order to reduce the number of debug messages in the serial monitor.
      delay(100);
    }

    // Disconnecting - restart advertising when no clients are connected
    if (connectedClients == 0 && deviceConnected) {
      delay(500);                   // give the bluetooth stack the chance to get things ready
      pServer->startAdvertising();  // restart advertising
      Serial.println("No clients connected, restarting advertising");
      deviceConnected = false;
    }

    // Connecting - update state when first client connects
    if (connectedClients > 0 && !deviceConnected) {
      // do stuff here on first connecting
      deviceConnected = true;
    }
  }
  vTaskDelete(NULL);
}