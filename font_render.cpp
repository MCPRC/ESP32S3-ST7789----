#include "core/font_render.h"
#include "core/draw_util.h"



void FontRender::drawASCII(uint16_t *fb, int x, int y, char c, uint16_t color) {
  uint8_t font[ASCII_8x16_BYTES];
  FontDriver::readFontData(getASCIIOffset(c), font, ASCII_8x16_BYTES);
  
  for (int row = 0; row < 16; row++) {
    uint8_t line = font[row];
    for (int bit = 0; bit < 8; bit++) {
      if (line & (0x80 >> bit)) {
        DrawUtil::setPixel(fb, x + bit, y + row, color);
      }
    }
  }
}

// 解析UTF-8字符
static uint32_t parseUtf8Char(const char*& str) {
  uint8_t c = (uint8_t)*str;
  uint32_t utf8 = 0;
  
  if (c < 0x80) {
    // 单字节ASCII字符
    utf8 = c;
    str++;
  } else if (c < 0xE0) {
    // 双字节UTF-8字符
    utf8 = ((c & 0x1F) << 6) | ((uint8_t)*(str+1) & 0x3F);
    str += 2;
  } else if (c < 0xF0) {
    // 三字节UTF-8字符（常用中文字符）
    utf8 = ((c & 0x0F) << 12) | (((uint8_t)*(str+1) & 0x3F) << 6) | ((uint8_t)*(str+2) & 0x3F);
    str += 3;
  } else {
    // 四字节UTF-8字符（不支持）
    str += 4;
  }
  
  return utf8;
}

void FontRender::drawString(uint16_t *fb, int x, int y, const char* str, uint16_t color) {
  const char* originalStr = str;
  
  while (*str) {
    // 解析UTF-8字符
    uint32_t utf8 = parseUtf8Char(str);
    
    if (utf8 < 0x80) {
      // ASCII字符，调用ASCII绘制方法
      drawASCII(fb, x, y, (char)utf8, color);
      x += 8; // ASCII字符宽度为8像素
    } else {
      // 中文字符，调用中文绘制方法
      uint8_t font[GB2312_16x16_BYTES];
      uint32_t offset = getHZ16OffsetFromUtf8(utf8);
      if (offset > 0) {
        // 从字库芯片读取字模数据
        FontDriver::readFontData(offset, font, GB2312_16x16_BYTES);
        
        // 绘制中文字符（16x16点阵）
        for (int row = 0; row < 16; row++) {
          // 每两行组成一个16位数据
          uint16_t line = (font[row*2] << 8) | font[row*2+1];
          for (int bit = 0; bit < 16; bit++) {
            // 检查当前位是否为1，如果是则绘制像素
            if (line & (0x8000 >> bit)) {
              DrawUtil::setPixel(fb, x + bit, y + row, color);
            }
          }
        }
        
        x += 16; // 中文字符宽度为16像素
      }
    }
  }
}

void FontRender::drawChinese16x16(uint16_t *fb, int x, int y, uint8_t area, uint8_t index, uint16_t color) {
  uint8_t font[GB2312_16x16_BYTES];
  FontDriver::readFontData(getHZ16Offset(area, index), font, GB2312_16x16_BYTES);
  
  for (int row = 0; row < 16; row++) {
    uint16_t line = (font[row*2] << 8) | font[row*2+1];
    for (int bit = 0; bit < 16; bit++) {
      if (line & (0x8000 >> bit)) {
        DrawUtil::setPixel(fb, x + bit, y + row, color);
      }
    }
  }
}

void FontRender::drawChinese16x16(uint16_t *fb, int x, int y, const char* chinese, uint16_t color) {
  // 假设输入的是GB2312编码的中文字符串
  while (*chinese && *(chinese+1)) {
    uint8_t area = *chinese;
    uint8_t index = *(chinese+1);
    drawChinese16x16(fb, x, y, area, index, color);
    x += 16;
    chinese += 2;
  }
}
