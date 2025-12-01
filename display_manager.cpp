#include "core/display_manager.h"

LCD1Driver DisplayManager::_lcd1;
LCD2Driver DisplayManager::_lcd2;

void DisplayManager::init() {
  _lcd1.init();
  _lcd2.init();
}

LCD1Driver& DisplayManager::getLCD1() {
  return _lcd1;
}

LCD2Driver& DisplayManager::getLCD2() {
  return _lcd2;
}

void DisplayManager::updateDisplays() {
  _lcd1.pushFrameBuffer(_lcd1.getFrameBuffer());
  _lcd2.pushFrameBuffer(_lcd2.getFrameBuffer());
}
