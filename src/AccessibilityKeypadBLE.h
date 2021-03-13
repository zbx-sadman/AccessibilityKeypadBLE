#pragma once
#define US_KEYBOARD 1

#include <Arduino.h>
#include "BLEDevice.h"
#include "BLEHIDDevice.h"
#include "HIDTypes.h"
#include "HIDKeyboardTypes.h"

#define TAG_BLE_ACCESSIBILITY_KEYPAD "AccessPad BLE";
#define NAME_MANUFACTURER "Prigodin Grigory"
#define NAME_DEVICE "AccessPad BLE"

// Message (report) sent when a key is pressed or released
typedef struct {
  uint8_t modifiers;          // bitmask: CTRL = 1, SHIFT = 2, ALT = 4
  uint8_t reserved;           // must be 0
  uint8_t pressedKeys[0x06];  // up to six concurrenlty pressed keys
} inputReport_t;

typedef struct {
  uint8_t inputPin;
  inputReport_t report;
} control_t;

// The report map describes the HID device (a keyboard in this case) and
// the messages (reports in HID terms) sent and received.
static const uint8_t REPORT_MAP[] = {
  USAGE_PAGE(1),      0x01,       // Generic Desktop Controls
  USAGE(1),           0x06,       // Keyboard
  COLLECTION(1),      0x01,       // Application
  REPORT_ID(1),       0x01,       //   Report ID (1)
  USAGE_PAGE(1),      0x07,       //   Keyboard/Keypad
  USAGE_MINIMUM(1),   0xE0,       //   Keyboard Left Control
  USAGE_MAXIMUM(1),   0xE7,       //   Keyboard Right Control
  LOGICAL_MINIMUM(1), 0x00,       //   Each bit is either 0 or 1
  LOGICAL_MAXIMUM(1), 0x01,
  REPORT_COUNT(1),    0x08,       //   8 bits for the modifier keys
  REPORT_SIZE(1),     0x01,
  HIDINPUT(1),        0x02,       //   Data, Var, Abs
  REPORT_COUNT(1),    0x01,       //   1 byte (unused)
  REPORT_SIZE(1),     0x08,
  HIDINPUT(1),        0x01,       //   Const, Array, Abs
  REPORT_COUNT(1),    0x06,       //   6 bytes (for up to 6 concurrently pressed keys)
  REPORT_SIZE(1),     0x08,
  LOGICAL_MINIMUM(1), 0x00,
  LOGICAL_MAXIMUM(1), 0x65,       //   101 keys
  USAGE_MINIMUM(1),   0x00,
  USAGE_MAXIMUM(1),   0x65,
  HIDINPUT(1),        0x00,       //   Data, Array, Abs
  REPORT_COUNT(1),    0x05,       //   5 bits (Num lock, Caps lock, Scroll lock, Compose, Kana)
  REPORT_SIZE(1),     0x01,
  USAGE_PAGE(1),      0x08,       //   LEDs
  USAGE_MINIMUM(1),   0x01,       //   Num Lock
  USAGE_MAXIMUM(1),   0x05,       //   Kana
  LOGICAL_MINIMUM(1), 0x00,
  LOGICAL_MAXIMUM(1), 0x01,
  HIDOUTPUT(1),       0x02,       //   Data, Var, Abs
  REPORT_COUNT(1),    0x01,       //   3 bits (Padding)
  REPORT_SIZE(1),     0x03,
  HIDOUTPUT(1),       0x01,       //   Const, Array, Abs
  END_COLLECTION(0)               // End application collection
};

const inputReport_t NO_KEY_PRESSED = { };

class AccessibilityKeypadBLE {
  public:
    AccessibilityKeypadBLE();
    class keypadCallbacks : public BLEServerCallbacks {
      public:
        keypadCallbacks(AccessibilityKeypadBLE* _parent): parent(_parent) {}
        void onConnect(BLEServer* server);
        void onDisconnect(BLEServer* server);
      private:
        AccessibilityKeypadBLE* parent;
    };
    void sendControl(inputReport_t& _report);
    uint8_t isConnected() { return connected; }
    uint8_t connected = false;
  private:
    BLEServer* server;
    BLEHIDDevice* hid;
    BLESecurity* security;
    BLECharacteristic* input;
    BLEAdvertising* advertising;;
};
