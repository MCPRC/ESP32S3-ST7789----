#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include "../drivers/lcd1_driver.h"
#include "../drivers/lcd2_driver.h"

class DisplayManager {
public:
  static void init();
  static LCD1Driver& getLCD1();
  static LCD2Driver& getLCD2();
  static void updateDisplays();
  
private:
  static LCD1Driver _lcd1;
  static LCD2Driver _lcd2;
};

#endif // DISPLAY_MANAGER_H
