#include "util/timer_util.h"
#include <Arduino.h>

// Timer 实现
Timer::Timer() {
  _startTime = 0;
  _elapsedTime = 0;
  _isRunning = false;
}

void Timer::start() {
  if (!_isRunning) {
    _startTime = millis();
    _isRunning = true;
  }
}

void Timer::stop() {
  if (_isRunning) {
    _elapsedTime = millis() - _startTime;
    _isRunning = false;
  }
}

void Timer::reset() {
  _startTime = millis();
  _elapsedTime = 0;
}

uint32_t Timer::elapsed() {
  if (_isRunning) {
    return millis() - _startTime;
  } else {
    return _elapsedTime;
  }
}

bool Timer::isRunning() {
  return _isRunning;
}

// PeriodicTimer 实现
PeriodicTimer::PeriodicTimer(uint32_t period) {
  _period = period;
  _lastTime = millis();
}

void PeriodicTimer::setPeriod(uint32_t period) {
  _period = period;
}

bool PeriodicTimer::check() {
  uint32_t now = millis();
  if (now - _lastTime >= _period) {
    _lastTime = now;
    return true;
  }
  return false;
}

void PeriodicTimer::reset() {
  _lastTime = millis();
}
