#include "drivers/lcd1_driver.h"

LCD1Driver::LCD1Driver() : ST7789Base(LCD1_CS, LCD1_DC, LCD1_RST, LCD1_BL) {
}
