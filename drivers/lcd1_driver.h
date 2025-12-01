#ifndef LCD1_DRIVER_H
#define LCD1_DRIVER_H

#include "st7789_base.h"
#include "../pins/lcd1_pins.h"

class LCD1Driver : public ST7789Base {
public:
  LCD1Driver();
};

#endif // LCD1_DRIVER_H
