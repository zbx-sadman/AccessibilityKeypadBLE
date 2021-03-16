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
#include "src\sound.h"

//*******************************************************************//
//                            Config area                            //
//*******************************************************************//

// ************ Buttons settings ************
// Activate Pull-Up on inputs, no external resistors need
const uint8_t inputMode = INPUT_PULLUP;
// Button bounces ignored for 10ms
const uint8_t debounceInterval = 10;
// Keycode repeated every 250ms if button still pressed
const uint32_t repeatsendKeyInterval = 250;

// ************ Sound settings ************
// Uncomment for using sound notification feature
#define FEATURE_DEVICE_BUZZING_ENABLE

// Type of sound device. Use SOUND_NONE for sound off and SOUND_PASSIVE_BUZZER for use passive buzzer
//const uint8_t soundDeviceType = SOUND_PASSIVE_BUZZER; 
const uint8_t soundDeviceType = SOUND_NONE; 
// Buzzer pin, use any number if sound device is SOUND_NONE 
const int8_t buzzerPin = 26;
// Level on the pin, which activate buzzer
const uint8_t passiveBuzzerOnLevel = HIGH;

// ************ Illumination settings ************
const uint8_t statusLedPin = LED_BUILTIN;

// ************ BLE pairing settings ************
// Uncomment for restrict access with "BLE Pairing by button" function.
#define FEATURE_DEVICE_PAIRING_ENABLE
// "Start pairing process" button's GPIO #
const uint8_t pairingButtonPin = 12;
//const uint8_t pairingButtonPin = 14;
// How much milliseconds to wait before stopping process if remote device ignore pairing offer
const uint32_t pairingTimeout = 15000;
// Blink / beep Interval while pairing is performed
const uint32_t pairingSignalizeInterval = 500;

// ************ Work mode settings ************
// Quickly change keymap (see or edit variants.h)
// Choose only one.
//#define KEYMAP_SWITCH_ACCESS_ANY_BOARD
#define KEYMAP_TALKBACK_MH_ET_LIVE_MINIKIT_ESP32
// #define KEYMAP_TALKBACK_ESP32_WROOM_32
#include "keymaps.h"

//*******************************************************************//
//                            System area                            //
//*******************************************************************//
#define LOG_TAG ""
AccessibilityKeypadBLE* accessibilityKeypad;
button_t button[arraySize(keyMap)];
SoundDevice* soundDevice;
typedef enum : uint8_t {psIdle, psInAction, psFinalize} pairingStage_t;

#if defined(FEATURE_DEVICE_PAIRING_ENABLE)
Bounce pairingButton;
#endif

void setup() {
  ESP_LOGE(LOG_TAG, "\n ***** System started *****\n");
  //  Serial.begin(115200);

  // configure pin for button
  for (uint8_t i = 0x00; arraySize(keyMap) > i; i++) {
    yield();
    pinMode(keyMap[i].inputPin, inputMode);
    button[i].obj = Bounce(keyMap[i].inputPin, debounceInterval);
  }
  pinMode(statusLedPin, OUTPUT);
  turnLedOff(statusLedPin);

  accessibilityKeypad = new AccessibilityKeypadBLE();

  soundDevice = new SoundDevice();
  soundDevice->configure(soundDeviceType, buzzerPin, passiveBuzzerOnLevel);
  soundDevice->turnOff();

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
    for (uint8_t i = 0x00; arraySize(keyMap) > i; i++) {
      yield();
      button[i].obj.update();
      if (button[i].obj.fell() || (LOW == button[i].obj.read() && millis() - button[i].lastPressedTime > repeatsendKeyInterval)) {
        turnLedOn(statusLedPin);
        accessibilityKeypad->sendKey(keyMap[i].report);
        turnLedOff(statusLedPin);
        button[i].lastPressedTime = millis();
      }
    }
  }

  //**** Pairing function block ****
#if defined(FEATURE_DEVICE_PAIRING_ENABLE)
  static uint32_t pairingStartTime, pairingLastSignalTime;
  static uint8_t pairingSignalOn;
  static pairingStage_t pairingStage = psIdle;
  pairingButton.update();

  // Pairing processing state machine
  switch (pairingStage) {
    case psIdle: {
        if (!pairingButton.fell()) {
          break;
        }
        accessibilityKeypad->startPairing();
        pairingLastSignalTime = pairingStartTime = millis();
        pairingStage = psInAction;
        break;
      } // case psIdle

    case psInAction: {
        if (accessibilityKeypad->isPaired()) {
          pairingStage = psFinalize;
        }
        if (pairingTimeout < millis() - pairingStartTime) {
          accessibilityKeypad->stopPairing();
          pairingStage = psFinalize;
        }
        if (pairingSignalizeInterval < millis() - pairingLastSignalTime) {
          pairingSignalOn = !pairingSignalOn;
          pairingSignalOn ? turnLedOn(statusLedPin) : turnLedOff(statusLedPin);
          pairingSignalOn ? soundDevice->turnOn() : soundDevice->turnOff();
          pairingLastSignalTime = millis();
        }
        break;
      } // case psInAction

    case psFinalize: {
        turnLedOff(statusLedPin);
        soundDevice->turnOff();
        pairingStage = psIdle;
        break;
      } // case psFinalize
  } // switch (pairingStage)

#endif
}
