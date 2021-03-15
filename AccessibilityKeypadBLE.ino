/*
   Accessibility Keypad / BLE

   Copyright (C) Prigodin Grigory (wrk.sadman@gmail.com)

   Release 1.0 , March 2021

   Based on sample of Manuel Bl (https://gist.github.com/manuelbl) (c) 2019 Manuel Bl

   Licensed under MIT License
   https://opensource.org/licenses/MIT
*/

/*

   Simple wiring nees: just connect one pin of button to GPIO from the config block (see below), and connect another pin of button to GND on dev-board.

*/

#include "src\Bounce2\Bounce2.h"
#include "src\AccessibilityKeypadBLE\AccessibilityKeypadBLE.h"
#include "src\util.h"

//*******************************************************************//
//                            Config area                            //
//*******************************************************************//
// Activate Pull-Up on inputs, no external resistors need
const uint8_t inputMode = INPUT_PULLUP;
// Button bounces ignored for 10ms
const uint8_t debounceInterval = 10;
// Keycode repeated every 250ms if button still pressed
const uint32_t repeatSendControlInterval = 250;

// Uncomment for restrict access with "BLE Pairing by button" function.
#define FEATURE_DEVICE_PAIRING_ENABLE
// "Start pairing process" button's GPIO #
const uint8_t pairingButtonPin = 14;
// How much milliseconds to wait before stopping process if remote device ignore pairing offer
const uint32_t pairingTimeout = 15000;
// Blink / beep Interval while pairing is performed
const uint32_t pairingSignalizeInterval = 500;


// Uncomment if you want quick move to configuration block for Android Switch Access feature
//#define SWITCH_ACCESS

/*
  https://support.google.com/accessibility/android/answer/6110948?hl=en#zippy=%2Cdefault-keymap



  // For MH-ET LIVE D1 mini ESP32
  https://i0.wp.com/www.bizkit.ru/wp-content/uploads/2019/12/MH-ET_LIVE_D1_mini_ESP32_pinout.png
  GPIO # | Board mark | Description               | Control         | Shortcut / gesture
  2      | IO2        | State                     | LED (Onboard)   |
  27     | IO27       | Home                      | Button (White)  | Alt+Crl+h
  32     | IO32       | Prev object               | Button (Green)  | Alt+Left  / swipe left
  12     | TDI        | Enter                     | Button (Yellow) | Alt+Enter / double tap
  17     | IO17       | Next Object               | Button (Red)    | Alt+Right / swipe right
  16     | IO16       | Read from the next object | Button (Blue)   | Alt+Ctrl+Shift+Enter
  15     | TD0        |
  13     | TCK        |
  14     | TMS        |
  33     | IO33       |
  26     | IO26       |

*/

#if defined(SWITCH_ACCESS)
// Switch Access controls. You can use just one button
control_t control[] = {
  // 27, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, // h
  // 32, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, // Left
  12, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, // Enter
  // 17, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, // Right
  // 16, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, // Enter
};

#else
// Talkback controls. Add some button's config items if you need

// For MH-ET LIVE D1 mini ESP32
control_t control[] = {
  27, (KEY_ALT | KEY_CTRL), 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00,             // Alt+Crl+h
  32, KEY_ALT, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Alt+Left
  12, KEY_ALT, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Alt+Enter
  17, KEY_ALT, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Alt+Right
  16, (KEY_ALT | KEY_CTRL | KEY_SHIFT), 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, // Alt+Ctrl+Shift+Enter
};

// For ESP32-WROOM-32 dev-board
/*
  control_t control[] = {
  12, (KEY_ALT | KEY_CTRL), 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00,             // Alt+Crl+h
  13, KEY_ALT, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Alt+Left
  14, KEY_ALT, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Alt+Enter
  26, KEY_ALT, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Alt+Right
  27, (KEY_ALT | KEY_CTRL | KEY_SHIFT), 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, // Alt+Ctrl+Shift+Enter
  };
*/
#endif

//*******************************************************************//
//                            System area                            //
//*******************************************************************//
#define LOG_TAG ""
AccessibilityKeypadBLE* accessibilityKeypad;
button_t button[arraySize(control)];
#if defined(FEATURE_DEVICE_PAIRING_ENABLE)
Bounce pairingButton;
#endif

void setup() {
  ESP_LOGE(LOG_TAG, "\n ***** System started *****\n");
  Serial.begin(115200);


  // configure pin for button
  for (uint8_t i = 0x00; arraySize(control) > i; i++) {
    yield();
    pinMode(control[i].inputPin, inputMode);
    button[i].obj = Bounce(control[i].inputPin, debounceInterval);
  }
  pinMode(LED_BUILTIN, OUTPUT);
  turnLedOff(LED_BUILTIN);

  accessibilityKeypad = new AccessibilityKeypadBLE();

#if defined(FEATURE_DEVICE_PAIRING_ENABLE)
  pinMode(pairingButtonPin, inputMode);
  pairingButton = Bounce(pairingButtonPin, debounceInterval);
  accessibilityKeypad->requirePairing(true);
#endif

}

void loop() {
  yield();

  //**** Button handling block ****
  if (accessibilityKeypad->isConnected()) {
    for (uint8_t i = 0x00; arraySize(control) > i; i++) {
      yield();
      button[i].obj.update();
      if (button[i].obj.fell() || (LOW == button[i].obj.read() && millis() - button[i].lastPressedTime > repeatSendControlInterval)) {
        turnLedOn(LED_BUILTIN);
        accessibilityKeypad->sendControl(control[i].report);
        turnLedOff(LED_BUILTIN);
        button[i].lastPressedTime = millis();
      }
    }
  }

  //**** Pairing function block ****
#if defined(FEATURE_DEVICE_PAIRING_ENABLE)
  static uint32_t pairingStartTime, pairingLastSignalTime;
  static uint8_t pairingInAction, pairingSignalOn;
  pairingButton.update();
  if (pairingButton.fell()) {
    accessibilityKeypad->startPairing();
    pairingLastSignalTime = pairingStartTime = millis();
    pairingInAction = true;
    pairingSignalOn = false;
  }

  if (accessibilityKeypad->isPaired()) {
    pairingInAction = false;
  }

  if (pairingInAction) {
    if (pairingTimeout < millis() - pairingStartTime) {
      accessibilityKeypad->stopPairing();
      turnLedOff(LED_BUILTIN);
      pairingInAction = false;
    }
    if (pairingSignalizeInterval < millis() - pairingLastSignalTime) {
      pairingSignalOn = !pairingSignalOn;
      pairingSignalOn ? turnLedOn(LED_BUILTIN) : turnLedOff(LED_BUILTIN);
      pairingLastSignalTime = millis();
    }
  }
#endif
}
