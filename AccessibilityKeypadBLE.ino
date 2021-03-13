/*
   Accessibility Keypad / BLE

   Copyright (C) Prigodin Grigory (wrk.sadman@gmail.com)

   Release 1.0 , March 2021

   Based on sample of Manuel Bl (https://gist.github.com/manuelbl) (c) 2019 Manuel Bl

   Licensed under MIT License
   https://opensource.org/licenses/MIT
*/


#include "src\Bounce2\Bounce2.h"
#include "src\AccessibilityKeypadBLE.h"
#include "util.h"

//*******************************************************************//
//                            Config area                            //
//*******************************************************************//
const uint8_t inputMode = INPUT_PULLUP;
const uint8_t debounceInterval = 10;
const uint32_t repeatSendControlInterval = 250;

/*
  https://i0.wp.com/www.bizkit.ru/wp-content/uploads/2019/12/MH-ET_LIVE_D1_mini_ESP32_pinout.png
  https://support.google.com/accessibility/android/answer/6110948?hl=en#zippy=%2Cdefault-keymap

  GPIO # | Board mark | Description               | Control         | Shortcut / gesture
  2      | IO2        | State                     | LED (Onboard)   |
  27     | IO27       | Home                      | Button (White)  | Alt+Crl+h
  32     | IO32       | Prev object               | Button (Green)  | Alt+Left  / swipe left
  12     | TDI        | Enter                     | Button (Yellow) | Alt+Enter / double tap
  17     | IO17       | Next Object               | Button (Red)    | Alt+Right / swipe right
  16     | IO16       | Read from the next object | Button (Blue)   | Alt+Ctrl+Shift+Enter
  15     | TD0        |                           | 

  13     | TCK        |
  14     | TMS        |
  33     | IO33       |
  26     | IO26       |

*/

//#define SWITCH_ACCESS

#if defined(SWITCH_ACCESS)
// Switch Access controls
control_t control[] = {
  27, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, // h
  32, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, // Left
  12, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, // Enter
  17, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, // Right
  //  16, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, // Enter
};
#else
// Talkback controls
control_t control[] = {
  27, (KEY_ALT | KEY_CTRL), 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00,             // Alt+Crl+h
  32, KEY_ALT, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Alt+Left
  12, KEY_ALT, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Alt+Enter
  17, KEY_ALT, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Alt+Right
  16, (KEY_ALT | KEY_CTRL | KEY_SHIFT), 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, // Alt+Ctrl+Shift+Enter
};
#endif

//*******************************************************************//
//                            System area                            //
//*******************************************************************//
#define LOG_TAG ""
AccessibilityKeypadBLE* accessibilityKeypad;
button_t button[arraySize(control)];

void setup() {
  ESP_LOGE(LOG_TAG, "\n ***** System started *****\n");

  // configure pin for button
  for (uint8_t i = 0x00; arraySize(control) > i; i++) {
    pinMode(control[i].inputPin, inputMode);
    //button[i].obj.attach(control[i].inputPin, debounceInterval);
    button[i].obj = Bounce(control[i].inputPin, debounceInterval);
  }
  pinMode(LED_BUILTIN, OUTPUT);
  turnLedOff(LED_BUILTIN);
  accessibilityKeypad = new AccessibilityKeypadBLE();
}

void loop() {
  if (accessibilityKeypad->isConnected()) {
    for (uint8_t i = 0x00; arraySize(control) > i; i++) {
      button[i].obj.update();
      //ESP_LOGI(LOG_TAG, "Button %d state: %d", i, button[i].obj.read());
      if (button[i].obj.fell() || (LOW == button[i].obj.read() && millis() - button[i].lastPressedTime > repeatSendControlInterval)) {
        turnLedOn(LED_BUILTIN);
        accessibilityKeypad->sendControl(control[i].report);
        turnLedOff(LED_BUILTIN);
        button[i].lastPressedTime = millis();
      }
    }
  }
  yield();
}
