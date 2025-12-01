#ifndef FONT_DRIVER_H
#define FONT_DRIVER_H

#include "spi_wrapper.h"
#include "hw_abstraction.h"
#include "../pins/font_pins.h"

class FontDriver {
public:
  static void init();
  static void readFontData(uint32_t addr, uint8_t* buf, uint8_t len);
  
private:
  static void selectFontFlash();
  static void deselectFontFlash();
};

#endif // FONT_DRIVER_H
