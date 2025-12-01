#include "protocol/command_executor.h"
#include "core/display_manager.h"
#include "core/draw_util.h"
#include "core/font_render.h"

// 初始化应用数组
AppManager::AppFunction AppManager::_apps[256] = {nullptr};
xTaskHandle AppManager::_currentTaskHandle = nullptr;
uint8_t AppManager::_currentAppId = 0;
bool AppManager::_isAppRunning = false;

// 应用任务函数
static void appTask(void *parameter) {
  uint8_t appId = *(uint8_t*)parameter;
  
  Serial.printf("AppTask started for app ID: 0x%02X\n", appId);
  
  // 直接调用应用函数
  AppManager::AppFunction appFunc;
  {
    // 通过静态方法获取应用函数，避免直接访问私有成员
    appFunc = AppManager::getAppFunction(appId);
  }
  if (appFunc != nullptr) {
    appFunc();
  }
  
  Serial.printf("AppTask finished for app ID: 0x%02X\n", appId);
  
  // 释放参数内存
  delete (uint8_t*)parameter;
  
  // 通过静态方法更新应用状态
  AppManager::setAppRunning(false);
  AppManager::setCurrentTaskHandle(nullptr);
  
  // 删除自身任务
  vTaskDelete(nullptr);
}

void AppManager::registerApp(uint8_t appId, AppFunction appFunc) {
  _apps[appId] = appFunc;
}

AppManager::AppFunction AppManager::getAppFunction(uint8_t appId) {
  return _apps[appId];
}

void AppManager::setAppRunning(bool running) {
  _isAppRunning = running;
}

void AppManager::setCurrentTaskHandle(xTaskHandle handle) {
  _currentTaskHandle = handle;
}

void AppManager::runApp(uint8_t appId) {
  Serial.printf("AppManager::runApp called with app ID: 0x%02X\n", appId);
  
  // 停止当前运行的应用
  stopCurrentApp();
  
  if (_apps[appId] != nullptr) {
    Serial.printf("Starting app with ID: 0x%02X\n", appId);
    
    // 更新应用状态
    _isAppRunning = true;
    _currentAppId = appId;
    
    // 创建新任务
    uint8_t *appIdPtr = new uint8_t(appId);
    BaseType_t result = xTaskCreate(
      appTask,         // 任务函数
      "AppTask",       // 任务名称
      8192,            // 任务栈大小，增加栈大小
      appIdPtr,        // 任务参数
      1,               // 任务优先级
      &_currentTaskHandle // 任务句柄
    );
    
    if (result != pdPASS) {
      Serial.printf("Failed to create task for app ID: 0x%02X\n", appId);
      _isAppRunning = false;
      delete appIdPtr;
    }
  }
}

void AppManager::stopCurrentApp() {
  if (_currentTaskHandle != nullptr) {
    Serial.printf("Stopping current app ID: 0x%02X\n", _currentAppId);
    
    // 终止当前任务
    vTaskDelete(_currentTaskHandle);
    _currentTaskHandle = nullptr;
    
    // 更新应用状态
    _isAppRunning = false;
    _currentAppId = 0;
    
    // 清屏为黑色
    LCD1Driver& lcd1 = DisplayManager::getLCD1();
    LCD2Driver& lcd2 = DisplayManager::getLCD2();
    uint16_t* fb1 = lcd1.getFrameBuffer();
    uint16_t* fb2 = lcd2.getFrameBuffer();
    DrawUtil::clear(fb1, 0x0000);
    DrawUtil::clear(fb2, 0x0000);
    DisplayManager::updateDisplays();
  }
}

void CommandExecutor::executeCommand(const Packet &packet) {
  Serial.printf("CommandExecutor::executeCommand() called with command: 0x%02X, data length: %d\n", packet.command, packet.dataLength);
  
  switch (packet.command) {
    case CMD_RUN_APP:
      if (packet.dataLength >= 1) {
        uint8_t appId = packet.data[0];
        Serial.printf("Running app with ID: 0x%02X\n", appId);
        runApp(appId);
      }
      break;
    
    default:
      Serial.printf("Unknown command: 0x%02X\n", packet.command);
      break;
  }
}

void CommandExecutor::runApp(uint8_t appId) {
  AppManager::runApp(appId);
}
