#include "apps/demo07/demo07.h"
#include "core/display_manager.h"
#include "core/draw_util.h"
#include "core/font_render.h"
#include "protocol/command_executor.h"
// 重命名底图变量，避免冲突
#define image_1bit image_1bit_bg
#include "素材/yuan.c"
#undef image_1bit

// 重命名顶图变量，避免冲突
#define image_1bit image_1bit_fg
#include "素材/xin.c"
#undef image_1bit

// 绘制1位深度图片的函数 - 静态函数，只在当前文件可见
static void draw1BitImage(uint16_t* fb, int x, int y, int width, int height, const unsigned char* imageData, uint16_t color) {
  for (int dy = 0; dy < height; dy++) {
    for (int dx = 0; dx < width; dx++) {
      // 计算字节偏移量和位偏移量
      int byteOffset = (dy * width + dx) / 8;
      int bitOffset = 7 - (dx % 8);
      
      // 检查像素是否为1
      if (imageData[byteOffset] & (1 << bitOffset)) {
        DrawUtil::setPixel(fb, x + dx, y + dy, color);
      }
    }
  }
}

void demo07_app() {
  // 获取屏幕驱动
  LCD1Driver& lcd1 = DisplayManager::getLCD1();
  LCD2Driver& lcd2 = DisplayManager::getLCD2();
  
  // 获取帧缓存
  uint16_t* fb1 = lcd1.getFrameBuffer();
  uint16_t* fb2 = lcd2.getFrameBuffer();
  
  // 运行10秒后退出
  uint32_t startTime = millis();
  
  // 屏幕分辨率
  const int SCREEN_WIDTH = 240;
  const int SCREEN_HEIGHT = 240;
  
  // 图片参数
  const int IMAGE_WIDTH = 240;
  const int IMAGE_HEIGHT = 240;
  
  // 颜色
  const uint16_t BG_COLOR = 0xE897; // 红蓝满色（底图颜色）
  const uint16_t FG_COLOR = 0xF800; // 红色（顶图颜色）
  
  // 清空屏幕
  DrawUtil::clear(fb1, 0x0000);
  DrawUtil::clear(fb2, 0x0000);
  
  // 绘制底图（圆形）
  draw1BitImage(fb1, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, image_1bit_bg, BG_COLOR);
  draw1BitImage(fb2, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, image_1bit_bg, BG_COLOR);
  
  // 绘制顶图（爱心）
  draw1BitImage(fb1, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, image_1bit_fg, FG_COLOR);
  draw1BitImage(fb2, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, image_1bit_fg, FG_COLOR);
  
  // 更新显示
  DisplayManager::updateDisplays();
  
  // 保持显示10秒
  while (millis() - startTime < 1000000) {
    // 静态显示，不需要更新
    delay(100);
  }
  
  // 清屏为黑色
  DrawUtil::clear(fb1, 0x0000);
  DrawUtil::clear(fb2, 0x0000);
  DisplayManager::updateDisplays();
}

// 注册应用
void register_demo07() {
  AppManager::registerApp(0x07, demo07_app);
}