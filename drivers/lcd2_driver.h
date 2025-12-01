#ifndef LCD2_DRIVER_H
#define LCD2_DRIVER_H

#include "st7789_base.h"
#include "../pins/lcd2_pins.h"

class LCD2Driver : public ST7789Base {
public:
  LCD2Driver();
};

#endif // LCD2_DRIVER_H
