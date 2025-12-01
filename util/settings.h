#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>

// 系统设置类
class Settings {
public:
  static void init();
  static void save();
  static void load();
  
  // 获取和设置设置值
  static uint8_t getBrightness();
  static void setBrightness(uint8_t brightness);
  
  static uint8_t getContrast();
  static void setContrast(uint8_t contrast);
  
  static uint8_t getDefaultApp();
  static void setDefaultApp(uint8_t appId);
  
private:
  // 设置数据结构
  struct SettingsData {
    uint8_t brightness;
    uint8_t contrast;
    uint8_t defaultApp;
    uint8_t reserved[23]; // 预留空间，总大小32字节
  };
  
  static SettingsData _data;
  static const char* _filename;
};

#endif // SETTINGS_H
