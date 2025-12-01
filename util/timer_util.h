#ifndef TIMER_UTIL_H
#define TIMER_UTIL_H

#include <stdint.h>

class Timer {
public:
  Timer();
  void start();
  void stop();
  void reset();
  uint32_t elapsed();
  bool isRunning();
  
private:
  uint32_t _startTime;
  uint32_t _elapsedTime;
  bool _isRunning;
};

class PeriodicTimer {
public:
  PeriodicTimer(uint32_t period);
  void setPeriod(uint32_t period);
  bool check();
  void reset();
  
private:
  uint32_t _period;
  uint32_t _lastTime;
};

#endif // TIMER_UTIL_H
