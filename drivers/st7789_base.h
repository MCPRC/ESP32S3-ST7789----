#ifndef ST7789_BASE_H
#define ST7789_BASE_H

#include "spi_wrapper.h"
#include "hw_abstraction.h"

class ST7789Base {
public:
  ST7789Base(int cs, int dc, int rst, int bl);
  void init();
  void clear(uint16_t color);
  void setPixel(int x, int y, uint16_t color);
  void pushFrameBuffer(uint16_t *fb);
  uint16_t* getFrameBuffer();
  
protected:
  void writeCmd(uint8_t cmd);
  void writeData(uint8_t data);
  void setFullAddr();
  
  int _cs;
  int _dc;
  int _rst;
  int _bl;
  uint16_t *_fb;
  const int _width = 240;
  const int _height = 240;
};

#endif // ST7789_BASE_H
