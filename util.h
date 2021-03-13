#pragma once
#include "src\Bounce2\Bounce2.h"

#define arraySize(_array) ( sizeof(_array) / sizeof(*(_array)) )

#if defined(DEBUG_ON)
#define DEBUG(x) x
#else
#define DEBUG(X) /* blank */
#endif

inline void turnLedOn(const uint8_t _pin) {
  digitalWrite(_pin, HIGH);
}

inline void turnLedOff(const uint8_t _pin) {
  digitalWrite(_pin, LOW);
}

typedef struct {
  Bounce obj;
  uint32_t lastPressedTime;
} button_t;
