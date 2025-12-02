#include "apps/demo04/demo04.h"
#include "core/display_manager.h"
#include "core/draw_util.h"
#include "core/font_render.h"
#include "protocol/command_executor.h"
#include <esp_system.h>
#include <esp_heap_caps.h>
#include <esp_flash.h>

void demo04_app() {
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
  
  // 处理器主频（ESP32S3默认主频为240MHz）
  uint32_t cpuFreq = getCpuFrequencyMhz();
  
  // 准备字符串缓冲区
  char infoStr[64] = {0};
  
  // 存储空间变量
  uint32_t flashSize = 0;
  
  while (millis() - startTime < 10000) {
    // 清屏为黑色
    DrawUtil::clear(fb1, 0x0000);
    DrawUtil::clear(fb2, 0x0000);
    
    // 绘制标题
    FontRender::drawString(fb1, 50, 20, "系统信息检测", 0xFFFF);
    FontRender::drawString(fb2, 50, 20, "系统信息检测", 0xFFFF);
    
    // 1. 处理器主频
    sprintf(infoStr, "CPU主频: %d MHz", cpuFreq);
    FontRender::drawString(fb1, 20, 60, infoStr, 0xFFFF);
    FontRender::drawString(fb2, 20, 60, infoStr, 0xFFFF);
    
    // 2. 处理器占用百分比
    // 简单实现：计算单次循环的时间
    uint32_t loopStart = millis();
    // 模拟一些计算
    for (int i = 0; i < 1000; i++) {
      volatile int j = i * i;
    }
    uint32_t loopEnd = millis();
    uint32_t loopTime = loopEnd - loopStart;
    float cpuUsage = (float)loopTime / 10.0 * 100.0; // 基于10ms延迟
    if (cpuUsage > 100.0) cpuUsage = 100.0;
    sprintf(infoStr, "CPU占用: %.1f%%", cpuUsage);
    FontRender::drawString(fb1, 20, 80, infoStr, 0xFFFF);
    FontRender::drawString(fb2, 20, 80, infoStr, 0xFFFF);
    
    // 3. 内存使用情况
    // 可用内存
    size_t freeHeap = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
    // 总内存
    size_t totalHeap = heap_caps_get_total_size(MALLOC_CAP_INTERNAL);
    // 内存使用率
    float memUsage = (float)(totalHeap - freeHeap) / totalHeap * 100.0;
    sprintf(infoStr, "内存使用: %.1f%%", memUsage);
    FontRender::drawString(fb1, 20, 100, infoStr, 0xFFFF);
    FontRender::drawString(fb2, 20, 100, infoStr, 0xFFFF);
    
    // 4. 存储空间使用情况
    // 简单实现：ESP32S3默认Flash大小为8MB
    flashSize = 8 * 1024 * 1024;
    sprintf(infoStr, "存储容量: %d MB", flashSize / (1024 * 1024));
    FontRender::drawString(fb1, 20, 120, infoStr, 0xFFFF);
    FontRender::drawString(fb2, 20, 120, infoStr, 0xFFFF);
    
    // 5. 更多内存信息
    sprintf(infoStr, "可用内存: %d KB", freeHeap / 1024);
    FontRender::drawString(fb1, 20, 140, infoStr, 0xFFFF);
    FontRender::drawString(fb2, 20, 140, infoStr, 0xFFFF);
    
    sprintf(infoStr, "总内存: %d KB", totalHeap / 1024);
    FontRender::drawString(fb1, 20, 160, infoStr, 0xFFFF);
    FontRender::drawString(fb2, 20, 160, infoStr, 0xFFFF);
    
    // 6. 应用信息
    FontRender::drawString(fb1, 20, 200, "demo04系统信息", 0xFFFF);
    FontRender::drawString(fb2, 20, 200, "demo04系统信息", 0xFFFF);
    
    // 更新显示
    DisplayManager::updateDisplays();
    
    // 延迟
    delay(1000); // 每秒更新一次
  }
  
  // 清屏为黑色
  DrawUtil::clear(fb1, 0x0000);
  DrawUtil::clear(fb2, 0x0000);
  DisplayManager::updateDisplays();
}

// 注册应用
void register_demo04() {
  AppManager::registerApp(0x04, demo04_app);
}
