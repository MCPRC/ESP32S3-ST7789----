#include "drivers/st7789_base.h"
#include "pins/lcd1_pins.h"
#include "pins/lcd2_pins.h"
#include "pins/font_pins.h"

ST7789Base::ST7789Base(int cs, int dc, int rst, int bl) {
  _cs = cs;
  _dc = dc;
  _rst = rst;
  _bl = bl;
}

void ST7789Base::init() {
  // 初始化引脚
  HWAbstraction::pinMode(_cs, OUTPUT);
  HWAbstraction::pinMode(_dc, OUTPUT);
  HWAbstraction::pinMode(_rst, OUTPUT);
  HWAbstraction::pinMode(_bl, OUTPUT);
  
  // 初始化背光
  HWAbstraction::digitalWrite(_bl, HIGH);
  
  // 复位屏幕
  HWAbstraction::digitalWrite(_rst, LOW);
  HWAbstraction::delay(80);
  HWAbstraction::digitalWrite(_rst, HIGH);
  HWAbstraction::delay(120);
  
  // 初始化ST7789
  writeCmd(0x11); // 退出睡眠模式
  HWAbstraction::delay(120);
  
  writeCmd(0x36); // 设置内存访问模式
  writeData(0x00);
  
  writeCmd(0x3A); // 设置像素格式
  writeData(0x55); // 16位RGB565
  
  writeCmd(0x21); // 反显关闭
  writeCmd(0x29); // 开启显示
  
  // 分配帧缓存
  _fb = (uint16_t*)heap_caps_malloc(_width * _height * 2, MALLOC_CAP_DMA);
  clear(0x0000);
}

void ST7789Base::writeCmd(uint8_t cmd) {
  // 禁用所有设备
  HWAbstraction::digitalWrite(LCD1_CS, HIGH);
  HWAbstraction::digitalWrite(LCD2_CS, HIGH);
  HWAbstraction::digitalWrite(FLASH_CS, HIGH);
  
  // 选择当前屏幕
  HWAbstraction::digitalWrite(_cs, LOW);
  HWAbstraction::digitalWrite(_dc, LOW);
  
  SPIWrapper::getSPI().transfer(cmd);
  
  HWAbstraction::digitalWrite(_cs, HIGH);
}

void ST7789Base::writeData(uint8_t data) {
  // 禁用所有设备
  HWAbstraction::digitalWrite(LCD1_CS, HIGH);
  HWAbstraction::digitalWrite(LCD2_CS, HIGH);
  HWAbstraction::digitalWrite(FLASH_CS, HIGH);
  
  // 选择当前屏幕
  HWAbstraction::digitalWrite(_cs, LOW);
  HWAbstraction::digitalWrite(_dc, HIGH);
  
  SPIWrapper::getSPI().transfer(data);
  
  HWAbstraction::digitalWrite(_cs, HIGH);
}

void ST7789Base::setFullAddr() {
  writeCmd(0x2A); // 设置列地址
  writeData(0);
  writeData(0);
  writeData(0);
  writeData(239);
  
  writeCmd(0x2B); // 设置行地址
  writeData(0);
  writeData(0);
  writeData(0);
  writeData(239);
  
  writeCmd(0x2C); // 写入内存
}

void ST7789Base::clear(uint16_t color) {
  for (int i = 0; i < _width * _height; i++) {
    _fb[i] = color;
  }
}

void ST7789Base::setPixel(int x, int y, uint16_t color) {
  if (x >= 0 && x < _width && y >= 0 && y < _height) {
    _fb[y * _width + x] = color;
  }
}

void ST7789Base::pushFrameBuffer(uint16_t *fb) {
  setFullAddr();
  
  // 禁用所有设备
  HWAbstraction::digitalWrite(LCD1_CS, HIGH);
  HWAbstraction::digitalWrite(LCD2_CS, HIGH);
  HWAbstraction::digitalWrite(FLASH_CS, HIGH);
  
  // 选择当前屏幕
  HWAbstraction::digitalWrite(_cs, LOW);
  HWAbstraction::digitalWrite(_dc, HIGH);
  
  SPIWrapper::getSPI().beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));
  SPIWrapper::getSPI().writePixels((uint8_t*)fb, _width * _height * 2);
  SPIWrapper::getSPI().endTransaction();
  
  HWAbstraction::digitalWrite(_cs, HIGH);
}

uint16_t* ST7789Base::getFrameBuffer() {
  return _fb;
}
