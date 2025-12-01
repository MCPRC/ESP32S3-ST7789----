#include "util/settings.h"
#include <EEPROM.h>

// 初始化静态成员
Settings::SettingsData Settings::_data;
const char* Settings::_filename = "/settings.bin";

void Settings::init() {
  // 初始化EEPROM
  EEPROM.begin(32);
  
  // 加载默认设置
  _data.brightness = 255;
  _data.contrast = 128;
  _data.defaultApp = 0x01; // 默认应用为demo01
  
  // 从EEPROM加载设置
  load();
}

void Settings::save() {
  // 将设置保存到EEPROM
  for (int i = 0; i < sizeof(SettingsData); i++) {
    EEPROM.write(i, ((uint8_t*)&_data)[i]);
  }
  EEPROM.commit();
}

void Settings::load() {
  // 从EEPROM加载设置
  for (int i = 0; i < sizeof(SettingsData); i++) {
    ((uint8_t*)&_data)[i] = EEPROM.read(i);
  }
}

uint8_t Settings::getBrightness() {
  return _data.brightness;
}

void Settings::setBrightness(uint8_t brightness) {
  _data.brightness = brightness;
  save();
}

uint8_t Settings::getContrast() {
  return _data.contrast;
}

void Settings::setContrast(uint8_t contrast) {
  _data.contrast = contrast;
  save();
}

uint8_t Settings::getDefaultApp() {
  return _data.defaultApp;
}

void Settings::setDefaultApp(uint8_t appId) {
  _data.defaultApp = appId;
  save();
}
