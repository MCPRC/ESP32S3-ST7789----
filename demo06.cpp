#include "apps/demo06/demo06.h"
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
static void draw1BitImage(uint16_t* fb, int x, int y, int width, int height, const unsigned char* imageData, uint16_t color, int scale = 1) {
  if (scale == 1) {
    // 原始大小绘制
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
  } else {
    // 缩放绘制（缩小）
    int scaledWidth = width / scale;
    int scaledHeight = height / scale;
    
    for (int dy = 0; dy < scaledHeight; dy++) {
      for (int dx = 0; dx < scaledWidth; dx++) {
        // 计算原始图像中的对应位置
        int srcX = dx * scale;
        int srcY = dy * scale;
        
        // 计算字节偏移量和位偏移量
        int byteOffset = (srcY * width + srcX) / 8;
        int bitOffset = 7 - (srcX % 8);
        
        // 检查像素是否为1
        if (imageData[byteOffset] & (1 << bitOffset)) {
          // 绘制缩放后的像素
          DrawUtil::fillRect(fb, x + dx, y + dy, 1, 1, color);
        }
      }
    }
  }
}

void demo06_app() {
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
  const int SCALED_WIDTH = IMAGE_WIDTH / 2;
  const int SCALED_HEIGHT = IMAGE_HEIGHT / 2;
  
  // 颜色
  const uint16_t BG_COLOR = 0xE897; // 红蓝满色（底图颜色）
  const uint16_t FG_COLOR = 0xF800; // 红色（顶图颜色）
  
  while (millis() - startTime < 1000000) {
    // 阶段1：显示原始大小的顶图
    DrawUtil::clear(fb1, 0x0000);
    DrawUtil::clear(fb2, 0x0000);
    
    // 绘制底图（圆形）
    draw1BitImage(fb1, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, image_1bit_bg, BG_COLOR);
    draw1BitImage(fb2, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, image_1bit_bg, BG_COLOR);
    
    // 绘制原始大小的顶图（爱心）
    draw1BitImage(fb1, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, image_1bit_fg, FG_COLOR);
    draw1BitImage(fb2, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, image_1bit_fg, FG_COLOR);
    
    // 绘制文字
//    FontRender::drawString(fb1, 70, 10, "原始大小", 0xFFFF);
//    FontRender::drawString(fb2, 70, 10, "原始大小", 0xFFFF);
    
    // 更新显示
    DisplayManager::updateDisplays();
    
    // 延迟500ms
    delay(500);
    
    // 阶段2：显示50%大小的顶图
    DrawUtil::clear(fb1, 0x0000);
    DrawUtil::clear(fb2, 0x0000);
    
    // 绘制底图（圆形）
    draw1BitImage(fb1, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, image_1bit_bg, BG_COLOR);
    draw1BitImage(fb2, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, image_1bit_bg, BG_COLOR);
    
    // 计算居中位置
    int scaledX = (SCREEN_WIDTH - SCALED_WIDTH) / 2;
    int scaledY = (SCREEN_HEIGHT - SCALED_HEIGHT) / 2;
    
    // 绘制50%大小的顶图（爱心）
    draw1BitImage(fb1, scaledX, scaledY, IMAGE_WIDTH, IMAGE_HEIGHT, image_1bit_fg, FG_COLOR, 2);
    draw1BitImage(fb2, scaledX, scaledY, IMAGE_WIDTH, IMAGE_HEIGHT, image_1bit_fg, FG_COLOR, 2);
    
    // 绘制文字
//    FontRender::drawString(fb1, 70, 10, "50%大小", 0xFFFF);
//    FontRender::drawString(fb2, 70, 10, "50%大小", 0xFFFF);
    
    // 更新显示
    DisplayManager::updateDisplays();
    
    // 延迟500ms
    delay(500);
  }
  
  // 清屏为黑色
  DrawUtil::clear(fb1, 0x0000);
  DrawUtil::clear(fb2, 0x0000);
  DisplayManager::updateDisplays();
}

// 注册应用
void register_demo06() {
  AppManager::registerApp(0x06, demo06_app);
}