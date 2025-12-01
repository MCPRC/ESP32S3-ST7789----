#ifndef FONT_RENDER_H
#define FONT_RENDER_H

#include <stdint.h>
#include <string.h>
#include "../drivers/font_driver.h"
#include "../drivers/font_index.h"

class FontRender {
public:
  static void drawASCII(uint16_t *fb, int x, int y, char c, uint16_t color);
  static void drawString(uint16_t *fb, int x, int y, const char* str, uint16_t color);
  static void drawChinese16x16(uint16_t *fb, int x, int y, uint8_t area, uint8_t index, uint16_t color);
  static void drawChinese16x16(uint16_t *fb, int x, int y, const char* chinese, uint16_t color);
  
private:
  static const int SCREEN_WIDTH = 240;
  static const int SCREEN_HEIGHT = 240;
};

#endif // FONT_RENDER_H
