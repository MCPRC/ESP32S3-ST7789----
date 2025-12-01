#include "drivers/spi_wrapper.h"

SPIClass SPIWrapper::spi1(1);

void SPIWrapper::init() {
  spi1.begin(PIN_SCK, PIN_MISO, PIN_MOSI);
}

SPIClass& SPIWrapper::getSPI() {
  return spi1;
}
