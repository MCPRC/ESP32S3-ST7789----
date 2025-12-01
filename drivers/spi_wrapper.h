#ifndef SPI_WRAPPER_H
#define SPI_WRAPPER_H

#include <SPI.h>
#include "../pins/bus_pins.h"

class SPIWrapper {
public:
  static void init();
  static SPIClass& getSPI();
  
private:
  static SPIClass spi1;
};

#endif // SPI_WRAPPER_H
