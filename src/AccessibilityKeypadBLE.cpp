#include "AccessibilityKeypadBLE.h"

AccessibilityKeypadBLE::AccessibilityKeypadBLE() {
   BLEDevice::init(NAME_DEVICE);
   server = BLEDevice::createServer();
   server->setCallbacks((BLEServerCallbacks*)new keypadCallbacks(this));
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
   security->setAuthenticationMode(ESP_LE_AUTH_BOND);
}


void AccessibilityKeypadBLE::keypadCallbacks::onConnect(BLEServer* server) {
   parent->connected = true;
   BLE2902* cccDesc = (BLE2902*)parent->input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
   cccDesc->setNotifications(true);
   ESP_LOGI(TAG_BLE_ACCESSIBILITY_KEYPAD, "Client has connected");
}

void AccessibilityKeypadBLE::keypadCallbacks::onDisconnect(BLEServer* server) {
   parent->connected = false;
   BLE2902* cccDesc = (BLE2902*)parent->input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
   cccDesc->setNotifications(false);
   ESP_LOGI(TAG_BLE_ACCESSIBILITY_KEYPAD, "Client has disconnected");
}

void AccessibilityKeypadBLE::sendControl(inputReport_t& _report) {
   ESP_LOGI(TAG_BLE_ACCESSIBILITY_KEYPAD, "Send control (mod | scan): %#02x | %#02x", _report.modifiers, _report.pressedKeys[0]);
   input->setValue((uint8_t*)&_report, sizeof(_report));
   input->notify();
   delay(5);
   input->setValue((uint8_t*)&NO_KEY_PRESSED, sizeof(NO_KEY_PRESSED));
   input->notify();
   delay(5);
}
