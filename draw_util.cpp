#include "core/draw_util.h"
#include <math.h>



void DrawUtil::clear(uint16_t *fb, uint16_t color) {
  for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
    fb[i] = color;
  }
}

void DrawUtil::setPixel(uint16_t *fb, int x, int y, uint16_t color) {
  if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
    fb[y * SCREEN_WIDTH + x] = color;
  }
}

void DrawUtil::drawLine(uint16_t *fb, int x1, int y1, int x2, int y2, uint16_t color) {
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int err = dx - dy;
  
  while (true) {
    setPixel(fb, x1, y1, color);
    
    if (x1 == x2 && y1 == y2) break;
    
    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y1 += sy;
    }
  }
}

void DrawUtil::drawRect(uint16_t *fb, int x, int y, int width, int height, uint16_t color) {
  drawLine(fb, x, y, x + width - 1, y, color);
  drawLine(fb, x + width - 1, y, x + width - 1, y + height - 1, color);
  drawLine(fb, x + width - 1, y + height - 1, x, y + height - 1, color);
  drawLine(fb, x, y + height - 1, x, y, color);
}

void DrawUtil::fillRect(uint16_t *fb, int x, int y, int width, int height, uint16_t color) {
  for (int i = y; i < y + height; i++) {
    for (int j = x; j < x + width; j++) {
      setPixel(fb, j, i, color);
    }
  }
}

void DrawUtil::drawCircle(uint16_t *fb, int x0, int y0, int radius, uint16_t color) {
  int x = radius;
  int y = 0;
  int err = 0;
  
  while (x >= y) {
    setPixel(fb, x0 + x, y0 + y, color);
    setPixel(fb, x0 + y, y0 + x, color);
    setPixel(fb, x0 - y, y0 + x, color);
    setPixel(fb, x0 - x, y0 + y, color);
    setPixel(fb, x0 - x, y0 - y, color);
    setPixel(fb, x0 - y, y0 - x, color);
    setPixel(fb, x0 + y, y0 - x, color);
    setPixel(fb, x0 + x, y0 - y, color);
    
    if (err <= 0) {
      y += 1;
      err += 2 * y + 1;
    }
    if (err > 0) {
      x -= 1;
      err -= 2 * x + 1;
    }
  }
}

void DrawUtil::fillCircle(uint16_t *fb, int x0, int y0, int radius, uint16_t color) {
  int x = radius;
  int y = 0;
  int err = 0;
  
  while (x >= y) {
    for (int i = x0 - x; i <= x0 + x; i++) {
      setPixel(fb, i, y0 + y, color);
      setPixel(fb, i, y0 - y, color);
    }
    for (int i = x0 - y; i <= x0 + y; i++) {
      setPixel(fb, i, y0 + x, color);
      setPixel(fb, i, y0 - x, color);
    }
    
    if (err <= 0) {
      y += 1;
      err += 2 * y + 1;
    }
    if (err > 0) {
      x -= 1;
      err -= 2 * x + 1;
    }
  }
}

uint16_t DrawUtil::rgbTo565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

uint16_t DrawUtil::wheelColor(uint16_t t) {
  uint8_t r = (sin(t * 0.05) * 127) + 128;
  uint8_t g = (sin(t * 0.08 + 2) * 127) + 128;
  uint8_t b = (sin(t * 0.13 + 4) * 127) + 128;
  return rgbTo565(r, g, b);
}
