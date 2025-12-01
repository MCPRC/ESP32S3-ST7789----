#include "protocol/command_parser.h"

bool CommandParser::parsePacket(uint8_t *buffer, int length, Packet &packet) {
  if (length < 6) return false; // 至少需要包头、命令、数据长度、数据内容、校验和、包尾
  
  int i = 0;
  
  // 寻找包头
  while (i < length && buffer[i] != PACKET_HEADER) {
    i++;
  }
  
  if (i >= length) return false;
  
  // 检查剩余长度是否足够
  if (i + 6 > length) return false;
  
  // 解析包头
  packet.header = buffer[i++];
  packet.command = buffer[i++];
  packet.dataLength = buffer[i++];
  
  // 检查数据长度是否合理
  if (packet.dataLength > 256) return false;
  
  // 检查剩余长度是否足够
  if (i + packet.dataLength + 2 > length) return false;
  
  // 解析数据
  for (int j = 0; j < packet.dataLength; j++) {
    packet.data[j] = buffer[i++];
  }
  
  // 解析校验和和包尾
  packet.checksum = buffer[i++];
  packet.tail = buffer[i++];
  
  // 验证包尾
  if (packet.tail != PACKET_TAIL) return false;
  
  // 验证校验和
  if (!verifyChecksum(packet)) return false;
  
  return true;
}

bool CommandParser::verifyChecksum(const Packet &packet) {
  uint8_t calculated = calculateChecksum(packet);
  return (calculated == packet.checksum);
}

uint8_t CommandParser::calculateChecksum(const Packet &packet) {
  uint8_t checksum = 0;
  
  checksum += packet.header;
  checksum += packet.command;
  checksum += packet.dataLength;
  
  for (int i = 0; i < packet.dataLength; i++) {
    checksum += packet.data[i];
  }
  
  return checksum;
}
