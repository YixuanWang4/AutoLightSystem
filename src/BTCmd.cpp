/*
This file is for the bluetooth command module of the Auto Light System project.
It is written in C++11 standard, using Arduino framework and Google Coding Style.
Maintainer: 
Finish your code here.
*/

#include <BTCmd.h>

static int s_readCount = 0;
static BLECharacteristic *s_pCharacteristic;

class MySecurityCallbacks : public BLESecurityCallbacks {
  bool onAuthorizationRequest(uint16_t connHandle, uint16_t attrHandle, bool isRead) {
    Serial.println("Authorization request received");
    if (isRead) {
      s_readCount++;
      // Keep value length <= (MTU - 1) to avoid a follow-up read request
      uint16_t maxLen = BLEDevice::getServer()->getPeerMTU(connHandle) - 1;
      String msg = "Authorized #" + String(s_readCount);
      if (msg.length() > maxLen) {
        msg = msg.substring(0, maxLen);
      }
      s_pCharacteristic->setValue(msg);
      // Grant authorization to the first 3 reads
      if (s_readCount <= 3) {
        Serial.println("Authorization granted");
        return true;
      } else {
        Serial.println("Authorization denied, read count exceeded");
        Serial.println("Please reset the read counter to continue");
        return false;
      }
    }
    // Fallback to deny
    Serial.println("Authorization denied");
    return false;
  }
};

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("设备已连接！");  // 连接成功时打印
      // 连接后可停止广播（可选）
    }

    void onDisconnect(BLEServer* pServer) {
      Serial.println("设备已断开连接！");  // 断开时打印
      // 断开后重新开始广播，允许再次连接
    }
};

//Function to initialize Bluetooth module, return true if successful
bool btInit() {
    BLEDevice::init("AutoLightSys100");
    return true;
}

bool btSetUp() {
  // Clear NVS to remove any cached pairing information
  // This ensures fresh authentication for testing
  Serial.println("Clearing NVS pairing data...");
  nvs_flash_erase();
  nvs_flash_init();

  Serial.print("Using BLE stack: ");
  Serial.println(BLEDevice::getBLEStackString());

  BLEDevice::init("BLE Auth Server");

  // Set MTU to 517 to avoid a follow-up read request
  BLEDevice::setMTU(517);

  // Configure BLE Security
  BLESecurity *pSecurity = new BLESecurity();

  // Set static passkey for authentication
  pSecurity->setPassKey(true, AUTH_PASSKEY);

  // Set IO capability to DisplayOnly for MITM authentication
  pSecurity->setCapability(ESP_IO_CAP_OUT);

  // Enable authorization requirements:
  // - bonding: true (for persistent storage of the keys)
  // - MITM: true (enables Man-In-The-Middle protection for password prompts)
  // - secure connection: true (enables secure connection for encryption)
  pSecurity->setAuthenticationMode(true, true, true);

  // Set the security callbacks
  BLEDevice::setSecurityCallbacks(new MySecurityCallbacks());

  // Create BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->advertiseOnDisconnect(true);
  pServer->setCallbacks(new MyServerCallbacks());
  // Create BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create characteristic with read and write properties
  uint32_t properties = BLECharacteristic::PROPERTY_READ;

  // For NimBLE: Add authentication properties
  // These properties ensure the characteristic requires authorization
  // (ignored by Bluedroid but harmless)
  properties |= BLECharacteristic::PROPERTY_READ_AUTHEN | BLECharacteristic::PROPERTY_READ_AUTHOR;

  s_pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, properties);

  // For Bluedroid: Set access permissions that require encryption and MITM
  // This ensures authorization is required (ignored by NimBLE)
  s_pCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENC_MITM | ESP_GATT_PERM_READ_AUTHORIZATION);

  // Set initial value
  s_pCharacteristic->setValue("Hello! You needed authorization to read this!");


  // Start the service
  pService->start();

  // Configure and start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // helps with iPhone connections
  pAdvertising->setMinPreferred(0x12);
  pAdvertising->setAppearance(0x0001);

  BLEDevice::startAdvertising();

  Serial.println("BLE Server is running!");
  Serial.println("Authorization is required to access the characteristic.");
  Serial.printf("Use passkey: %d when prompted\n", AUTH_PASSKEY);
  return true;
}

void btMaintain(void * parameter) {
  // Notify changed value to all connected clients
  while(true){
    vTaskDelay(1000);
  }
  vTaskDelete(NULL);
}