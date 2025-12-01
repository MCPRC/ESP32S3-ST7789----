#include "apps/demo03/demo03.h"
#include "core/display_manager.h"
#include "core/draw_util.h"
#include "core/font_render.h"
#include "protocol/command_executor.h"

void demo03_app() {
  // 获取屏幕驱动
  LCD1Driver& lcd1 = DisplayManager::getLCD1();
  LCD2Driver& lcd2 = DisplayManager::getLCD2();
  
  // 获取帧缓存
  uint16_t* fb1 = lcd1.getFrameBuffer();
  uint16_t* fb2 = lcd2.getFrameBuffer();
  
  // 运行10秒后退出
  uint32_t startTime = millis();
  int x1 = 0, y1 = 120;
  int x2 = 240, y2 = 120;
  int dx1 = 2, dy1 = 1;
  int dx2 = -2, dy2 = -1;
  
  while (millis() - startTime < 10000) {
    // 清屏为黑色
    DrawUtil::clear(fb1, 0x0000);
    DrawUtil::clear(fb2, 0x0000);
    
    // 绘制移动的圆形
    DrawUtil::fillCircle(fb1, x1, y1, 20, DrawUtil::wheelColor(millis() / 10));
    DrawUtil::fillCircle(fb2, x2, y2, 20, DrawUtil::wheelColor(millis() / 10 + 100));
    
    // 绘制文字
    FontRender::drawString(fb1, 50, 20, "Demo 03", 0xFFFF);
    FontRender::drawString(fb2, 50, 20, "Demo 03", 0xFFFF);
    
    FontRender::drawString(fb1, 20, 200, "Moving Circle", 0xFFFF);
    FontRender::drawString(fb2, 20, 200, "Moving Circle", 0xFFFF);
    
    // 更新显示
    DisplayManager::updateDisplays();
    
    // 更新圆形位置
    x1 += dx1;
    y1 += dy1;
    x2 += dx2;
    y2 += dy2;
    
    // 边界检测
    if (x1 <= 20 || x1 >= 220) dx1 = -dx1;
    if (y1 <= 20 || y1 >= 220) dy1 = -dy1;
    if (x2 <= 20 || x2 >= 220) dx2 = -dx2;
    if (y2 <= 20 || y2 >= 220) dy2 = -dy2;
    
    // 延迟
    delay(10);
  }
  
  // 清屏为黑色
  DrawUtil::clear(fb1, 0x0000);
  DrawUtil::clear(fb2, 0x0000);
  DisplayManager::updateDisplays();
}

// 注册应用
void register_demo03() {
  AppManager::registerApp(0x03, demo03_app);
}
