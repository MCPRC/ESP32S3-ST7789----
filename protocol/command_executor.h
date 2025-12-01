#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include "command_parser.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// 应用管理类
class AppManager {
public:
  typedef void (*AppFunction)();
  
  static void registerApp(uint8_t appId, AppFunction appFunc);
  static void runApp(uint8_t appId);
  static void stopCurrentApp();
  
  // 添加静态方法用于任务访问
  static AppFunction getAppFunction(uint8_t appId);
  static void setAppRunning(bool running);
  static void setCurrentTaskHandle(xTaskHandle handle);
  
private:
  static AppFunction _apps[256];
  static xTaskHandle _currentTaskHandle;
  static uint8_t _currentAppId;
  static bool _isAppRunning;
};

class CommandExecutor {
public:
  static void executeCommand(const Packet &packet);
  
private:
  static void runApp(uint8_t appId);
};

#endif // COMMAND_EXECUTOR_H
