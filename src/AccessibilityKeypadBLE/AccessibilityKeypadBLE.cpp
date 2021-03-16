#include "AccessibilityKeypadBLE.h"

AccessibilityKeypadBLE::AccessibilityKeypadBLE() {
   BLEDevice::init(NAME_DEVICE);
   BLEDevice::setSecurityCallbacks(new KeypadSecurityCallbacks(this));
   server = BLEDevice::createServer();
   server->setCallbacks((BLEServerCallbacks*)new KeypadServerCallbacks(this));
   hid = new BLEHIDDevice(server);
   input = hid->inputReport(1);
   hid->manufacturer()->setValue(NAME_MANUFACTURER);
   hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
   hid->hidInfo(0x00, 0x02);
   hid->reportMap((uint8_t*)REPORT_MAP, sizeof(REPORT_MAP));
   hid->startServices();

   advertising = server->getAdvertising();
   advertising->setAppearance(HID_KEYBOARD);
   advertising->addServiceUUID(hid->hidService()->getUUID());
   advertising->addServiceUUID(hid->deviceInfo()->getUUID());
   advertising->addServiceUUID(hid->batteryService()->getUUID());
   advertising->start();

   security = new BLESecurity();
   security->setKeySize();
   //security->setAuthenticationMode(ESP_LE_AUTH_BOND);
   security->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM_BOND);
   security->setCapability(ESP_IO_CAP_NONE);
}


void AccessibilityKeypadBLE::KeypadServerCallbacks::onConnect(BLEServer* server) {
   parent->connected = true;
   BLE2902* cccDesc = (BLE2902*)parent->input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
   cccDesc->setNotifications(true);
   ESP_LOGI(TAG_BLE_ACCESSIBILITY_KEYPAD, "Client has connected");
}

void AccessibilityKeypadBLE::KeypadServerCallbacks::onDisconnect(BLEServer* server) {
   parent->connected = false;
   BLE2902* cccDesc = (BLE2902*)parent->input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
   cccDesc->setNotifications(false);
   ESP_LOGI(TAG_BLE_ACCESSIBILITY_KEYPAD, "Client has disconnected");
}

void AccessibilityKeypadBLE::sendKey(inputReport_t& _report) {
   ESP_LOGI(TAG_BLE_ACCESSIBILITY_KEYPAD, "Send control (mod | scan): %#02x | %#02x", _report.modifiers, _report.pressedKeys[0x00]);
   input->setValue((uint8_t*)&_report, sizeof(_report));
   input->notify();
   delay(5);
   input->setValue((uint8_t*)&NO_KEY_PRESSED, sizeof(NO_KEY_PRESSED));
   input->notify();
   delay(5);
}


void AccessibilityKeypadBLE::startPairing() {
   ESP_LOGI(TAG_BLE_ACCESSIBILITY_KEYPAD, "Just called");
   if (needPairing) {
     enablePairing = true;
     donePairing = false; 
   } else {
     donePairing = true; 
   }
}

void AccessibilityKeypadBLE::stopPairing() {
   ESP_LOGI(TAG_BLE_ACCESSIBILITY_KEYPAD, "Just called");
   if (needPairing) {
     enablePairing = false;
   } else {
     donePairing = true; 
   }
}

uint8_t AccessibilityKeypadBLE::isPaired() {
   return needPairing ? donePairing : true; 
}

void AccessibilityKeypadBLE::requirePairing(const uint8_t _needPairing) {
   needPairing = _needPairing; 
}


bool AccessibilityKeypadBLE::KeypadSecurityCallbacks::onConfirmPIN(uint32_t _pinCode) { 
   ESP_LOGI(TAG_BLE_ACCESSIBILITY_KEYPAD, "onConfirmPIN, pinCode = %d", _pinCode);
   return false; 
}
uint32_t AccessibilityKeypadBLE::KeypadSecurityCallbacks::onPassKeyRequest() { 
   ESP_LOGI(TAG_BLE_ACCESSIBILITY_KEYPAD, "onPassKeyRequest");
   return 500341UL; 
}

void AccessibilityKeypadBLE::KeypadSecurityCallbacks::onPassKeyNotify(uint32_t _passKey) { 
   ESP_LOGI(TAG_BLE_ACCESSIBILITY_KEYPAD, "onPassKeyNotify, pinCode = %lu", _passKey);
}

bool AccessibilityKeypadBLE::KeypadSecurityCallbacks::onSecurityRequest() { 
   uint8_t rc = parent->needPairing ? parent->enablePairing : true; 
   ESP_LOGI(TAG_BLE_ACCESSIBILITY_KEYPAD, "onSecurityRequest, enable = %d", rc);
   return rc;
}

void AccessibilityKeypadBLE::KeypadSecurityCallbacks::onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl) { 
   parent->donePairing = (parent->enablePairing && cmpl.success); 
   parent->enablePairing = false;
   ESP_LOGI(TAG_BLE_ACCESSIBILITY_KEYPAD, "onAuthenticationComplete, success = %d", cmpl.success);
}

