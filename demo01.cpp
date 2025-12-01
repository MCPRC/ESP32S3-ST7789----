#include "apps/demo01/demo01.h"
#include "core/display_manager.h"
#include "core/draw_util.h"
#include "core/font_render.h"
#include "protocol/command_executor.h"

void demo01_app() {
  // 获取屏幕驱动
  LCD1Driver& lcd1 = DisplayManager::getLCD1();
  LCD2Driver& lcd2 = DisplayManager::getLCD2();
  
  // 获取帧缓存
  uint16_t* fb1 = lcd1.getFrameBuffer();
  uint16_t* fb2 = lcd2.getFrameBuffer();
  
  // RGB刷新三次屏幕
  for (int i = 0; i < 3; i++) {
    // 红色
    DrawUtil::clear(fb1, 0xF800);
    DrawUtil::clear(fb2, 0xF800);
    DisplayManager::updateDisplays();
    delay(500);
    
    // 绿色
    DrawUtil::clear(fb1, 0x07E0);
    DrawUtil::clear(fb2, 0x07E0);
    DisplayManager::updateDisplays();
    delay(500);
    
    // 蓝色
    DrawUtil::clear(fb1, 0x001F);
    DrawUtil::clear(fb2, 0x001F);
    DisplayManager::updateDisplays();
    delay(500);
  }
  
  // 清屏为黑色
  DrawUtil::clear(fb1, 0x0000);
  DrawUtil::clear(fb2, 0x0000);
  
  // 打印汉字：系统初始化完毕
  // 使用drawString方法，自动处理汉字编码和绘制
  FontRender::drawString(fb1, 60, 100, "系统初始化完毕", 0xFFFF); // 屏幕1: 在(60,100)位置显示"系统初始化完毕"，白色
  
  // 在屏幕2上也显示相同内容
  FontRender::drawString(fb2, 60, 100, "系统初始化完毕", 0xFFFF); // 屏幕2: 在(60,100)位置显示"系统初始化完毕"，白色
  
  // 更新显示
  DisplayManager::updateDisplays();
}

// 注册应用
void register_demo01() {
  AppManager::registerApp(0x01, demo01_app);
}
