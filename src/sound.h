#pragma once

#include <Arduino.h>

#define SOUND_NONE            0x00
#define SOUND_PASSIVE_BUZZER  0x01
#define SOUND_ACTIVE_BUZZER   0x02

#define SOUND_PIN_NONE        -0x01

class SoundDevice {
  public:
    SoundDevice() { }
    void configure(uint8_t, int8_t, uint8_t);
    void turnOn(uint8_t, uint32_t);
    void turnOn();
    void turnOff();
  private:
    uint8_t type        = SOUND_NONE;
    uint8_t pin         = SOUND_PIN_NONE;
    uint8_t activeLevel = HIGH;
};

void SoundDevice::configure(uint8_t _type, int8_t _pin, uint8_t _activeLevel) {
  if (SOUND_NONE == _type) { return; }

  type = _type;
  pin  = _pin;
  activeLevel = _activeLevel;
  pinMode(pin, OUTPUT);
}

void SoundDevice::turnOn() {
  if (SOUND_NONE == type) { return; }

  digitalWrite(pin, activeLevel);
}

// Need to add ESP32 tone inplementation
void SoundDevice::turnOn(uint8_t _frequency, uint32_t _duration) {
  if (SOUND_NONE == type) { return; }

//  tone(pin, _frequency, _duration);
}

void SoundDevice::turnOff() {
  if (SOUND_NONE == type) { return; }

//  noTone(pin);
  digitalWrite(pin, !activeLevel);
}

