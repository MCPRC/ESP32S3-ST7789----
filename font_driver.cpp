#include "drivers/font_driver.h"
#include "drivers/font_index.h"
#include "pins/lcd1_pins.h"
#include "pins/lcd2_pins.h"

void FontDriver::init() {
  HWAbstraction::pinMode(FLASH_CS, OUTPUT);
  HWAbstraction::digitalWrite(FLASH_CS, HIGH);
}

void FontDriver::readFontData(uint32_t addr, uint8_t* buf, uint8_t len) {
  selectFontFlash();
  
  SPIWrapper::getSPI().beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  SPIWrapper::getSPI().transfer(0x03); // 读命令
  SPIWrapper::getSPI().transfer(addr >> 16); // 地址高位
  SPIWrapper::getSPI().transfer(addr >> 8);  // 地址中位
  SPIWrapper::getSPI().transfer(addr);       // 地址低位
  
  for (int i = 0; i < len; i++) {
    buf[i] = SPIWrapper::getSPI().transfer(0x00);
  }
  
  SPIWrapper::getSPI().endTransaction();
  deselectFontFlash();
}

void FontDriver::selectFontFlash() {
  // 禁用所有其他设备
  HWAbstraction::digitalWrite(LCD1_CS, HIGH);
  HWAbstraction::digitalWrite(LCD2_CS, HIGH);
  HWAbstraction::digitalWrite(FLASH_CS, LOW);
}

void FontDriver::deselectFontFlash() {
  HWAbstraction::digitalWrite(FLASH_CS, HIGH);
}
