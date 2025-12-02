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
  
  // 屏幕分辨率（从代码中推断为240x240）
  const int SCREEN_WIDTH = 240;
  const int SCREEN_HEIGHT = 240;
  const int CENTER_X = SCREEN_WIDTH / 2;
  const int CENTER_Y = SCREEN_HEIGHT / 2;
  
  // 运行10秒后退出
  uint32_t startTime = millis();
  
  // 将圆初始位置设置到屏幕中间
  int x1 = CENTER_X, y1 = CENTER_Y;
  int x2 = CENTER_X, y2 = CENTER_Y;
  int dx1 = 2, dy1 = 1;
  int dx2 = -2, dy2 = -1;
  
  // FPS检测变量
  uint32_t fpsStartTime = millis();
  int frameCount = 0;
  float fps = 0.0;
  char fpsStr[16] = {0};
  
  while (millis() - startTime < 10000) {
    // 清屏为黑色
    DrawUtil::clear(fb1, 0x0000);
    DrawUtil::clear(fb2, 0x0000);
    
    // 绘制移动的圆形
    DrawUtil::fillCircle(fb1, x1, y1, 20, DrawUtil::wheelColor(millis() / 10));
    DrawUtil::fillCircle(fb2, x2, y2, 20, DrawUtil::wheelColor(millis() / 10 + 100));
    
    // 绘制文字
    FontRender::drawString(fb1, 50, 20, "屏幕1测试程序 03", 0xFFFF);
    FontRender::drawString(fb2, 50, 20, "屏幕2测试程序 03", 0xFFFF);
    
    FontRender::drawString(fb1, 20, 200, "移动的炫彩圆", 0xFFFF);
    FontRender::drawString(fb2, 20, 200, "移动的炫彩圆", 0xFFFF);
    
    // 更新显示
    DisplayManager::updateDisplays();
    
    // 更新圆形位置
    x1 += dx1;
    y1 += dy1;
    x2 += dx2;
    y2 += dy2;
    
    // 边界检测（确保圆在屏幕内移动）
    if (x1 <= 20 || x1 >= SCREEN_WIDTH - 20) dx1 = -dx1;
    if (y1 <= 20 || y1 >= SCREEN_HEIGHT - 20) dy1 = -dy1;
    if (x2 <= 20 || x2 >= SCREEN_WIDTH - 20) dx2 = -dx2;
    if (y2 <= 20 || y2 >= SCREEN_HEIGHT - 20) dy2 = -dy2;
    
    // FPS计算
    frameCount++;
    if (millis() - fpsStartTime >= 1000) {
      fps = (float)frameCount / ((millis() - fpsStartTime) / 1000.0);
      sprintf(fpsStr, "FPS: %.1f", fps);
      fpsStartTime = millis();
      frameCount = 0;
      
      // 打印FPS到串口
      Serial.printf("FPS: %.1f\n", fps);
    }
    
    // 绘制FPS
    FontRender::drawString(fb1, 10, 40, fpsStr, 0xFFFF);
    FontRender::drawString(fb2, 10, 40, fpsStr, 0xFFFF);
    
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