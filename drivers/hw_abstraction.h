#ifndef HW_ABSTRACTION_H
#define HW_ABSTRACTION_H

#include <Arduino.h>

class HWAbstraction {
public:
  static void pinMode(uint8_t pin, uint8_t mode);
  static void digitalWrite(uint8_t pin, uint8_t value);
  static uint8_t digitalRead(uint8_t pin);
  static void delay(uint32_t ms);
  static uint32_t millis();
};

#endif // HW_ABSTRACTION_H
