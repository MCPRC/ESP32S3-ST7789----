#include "drivers/hw_abstraction.h"

void HWAbstraction::pinMode(uint8_t pin, uint8_t mode) {
  ::pinMode(pin, mode);
}

void HWAbstraction::digitalWrite(uint8_t pin, uint8_t value) {
  ::digitalWrite(pin, value);
}

uint8_t HWAbstraction::digitalRead(uint8_t pin) {
  return ::digitalRead(pin);
}

void HWAbstraction::delay(uint32_t ms) {
  ::delay(ms);
}

uint32_t HWAbstraction::millis() {
  return ::millis();
}
