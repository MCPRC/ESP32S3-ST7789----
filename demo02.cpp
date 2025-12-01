#include "apps/demo02/demo02.h"
#include "core/display_manager.h"
#include "core/draw_util.h"
#include "core/font_render.h"
#include "protocol/command_executor.h"

void demo02_app() {
  Serial.println("demo02_app started");
  
  // 获取屏幕驱动
  LCD1Driver& lcd1 = DisplayManager::getLCD1();
  LCD2Driver& lcd2 = DisplayManager::getLCD2();
  
  // 获取帧缓存
  uint16_t* fb1 = lcd1.getFrameBuffer();
  uint16_t* fb2 = lcd2.getFrameBuffer();
  
  // 简单清屏和显示
  DrawUtil::clear(fb1, 0x001F);   // 蓝背景
  DrawUtil::clear(fb2, 0xF800);   // 红背景
  
  // 显示简单文字
  // drawString 参数说明：
  // 参数1: 帧缓冲区指针 (fb1 对应屏幕1, fb2 对应屏幕2)
  // 参数2: 显示起始X坐标 (像素)
  // 参数3: 显示起始Y坐标 (像素)
  // 参数4: 要显示的字符串 (支持ASCII和中文)
  // 参数5: 显示颜色 (16位RGB565格式)
  
  FontRender::drawString(fb1, 50, 100, "Demo02", 0xFFFF); // 屏幕1: 在(50,100)位置显示"Demo02"，白色
  FontRender::drawString(fb1, 30, 120, "Hello", 0xFFFF); // 屏幕1: 在(30,120)位置显示"Hello"，白色
  FontRender::drawString(fb1, 30, 140, "你好", 0xFFFF); // 屏幕1: 在(30,140)位置显示"你好"，白色
  FontRender::drawString(fb1, 30, 160, "你好这个世界", 0xFFFF); // 屏幕1: 在(30,160)位置显示"你好世界"，白色

  FontRender::drawString(fb2, 50, 100, "Demo02", 0xFFFF); // 屏幕2: 在(50,100)位置显示"Demo02"，白色
  FontRender::drawString(fb2, 30, 120, "World", 0xFFFF); // 屏幕2: 在(30,120)位置显示"World"，白色
  FontRender::drawString(fb2, 30, 140, "你好", 0xFFFF); // 屏幕2: 在(30,140)位置显示"你好"，白色
  FontRender::drawString(fb2, 30, 160, "你好世界", 0xFFFF); // 屏幕2: 在(30,160)位置显示"你好世界"，白色
  
  // 更新显示
  DisplayManager::updateDisplays();
  
  // 简单延迟，然后退出
  delay(5000);
  
  // 清屏为黑色
  DrawUtil::clear(fb1, 0x0000);
  DrawUtil::clear(fb2, 0x0000);
  DisplayManager::updateDisplays();
  
  Serial.println("demo02_app finished");
}

// 注册应用
void register_demo02() {
  AppManager::registerApp(0x02, demo02_app);
}
