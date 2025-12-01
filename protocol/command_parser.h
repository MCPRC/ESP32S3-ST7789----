#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stdint.h>
#include "packet_format.h"
#include "command_defines.h"

class CommandParser {
public:
  static bool parsePacket(uint8_t *buffer, int length, Packet &packet);
  static bool verifyChecksum(const Packet &packet);
  static uint8_t calculateChecksum(const Packet &packet);
};

#endif // COMMAND_PARSER_H
