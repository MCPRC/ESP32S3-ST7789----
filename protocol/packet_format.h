#ifndef PACKET_FORMAT_H
#define PACKET_FORMAT_H

#include <stdint.h>

// 数据包格式定义
#define PACKET_HEADER  0xAA
#define PACKET_TAIL    0x55

struct Packet {
  uint8_t header;    // 包头，固定为0xAA
  uint8_t command;   // 命令类型
  uint8_t dataLength;// 数据长度
  uint8_t data[256]; // 数据内容
  uint8_t checksum;  // 校验和
  uint8_t tail;      // 包尾，固定为0x55
};

#endif // PACKET_FORMAT_H
