#ifndef DRAW_UTIL_H
#define DRAW_UTIL_H

#include <stdint.h>

class DrawUtil {
public:
  static void clear(uint16_t *fb, uint16_t color);
  static void setPixel(uint16_t *fb, int x, int y, uint16_t color);
  static void drawLine(uint16_t *fb, int x1, int y1, int x2, int y2, uint16_t color);
  static void drawRect(uint16_t *fb, int x, int y, int width, int height, uint16_t color);
  static void fillRect(uint16_t *fb, int x, int y, int width, int height, uint16_t color);
  static void drawCircle(uint16_t *fb, int x0, int y0, int radius, uint16_t color);
  static void fillCircle(uint16_t *fb, int x0, int y0, int radius, uint16_t color);
  
  static uint16_t rgbTo565(uint8_t r, uint8_t g, uint8_t b);
  static uint16_t wheelColor(uint16_t t);
  
private:
  static const int SCREEN_WIDTH = 240;
  static const int SCREEN_HEIGHT = 240;
};

#endif // DRAW_UTIL_H
