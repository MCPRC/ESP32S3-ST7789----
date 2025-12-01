#include "drivers/lcd2_driver.h"

LCD2Driver::LCD2Driver() : ST7789Base(LCD2_CS, LCD2_DC, LCD2_RST, LCD2_BL) {
}
