#ifndef FONT_INDEX_H
#define FONT_INDEX_H

// 字库索引定义

// 常用字体默认值（从字库目录读取后会更新）
static uint32_t ASCII_8x16_START = 0x1D00;
static uint8_t ASCII_8x16_BYTES = 16;

static uint32_t GB2312_16x16_START = 0x71300;
static uint8_t GB2312_16x16_BYTES = 32;

// 常用字体宏定义
#define FONT16_START GB2312_16x16_START
#define FONT16_BYTES GB2312_16x16_BYTES

// UTF-8 到 GB2312 转换表（简化版，仅包含常用字）
static const struct {
  uint32_t utf8;      // UTF-8 码点
  uint16_t gb2312;    // GB2312 编码
} utf8_to_gb2312[] = {
  {0x4F60, 0xC4E3}, // 你
  {0x597D, 0xBAC3}, // 好
  {0x4E16, 0xCAC0}, // 世
  {0x754C, 0xBCF4}, // 界
  {0x7CFB, 0xCFB5}, // 系
  {0x7EDF, 0xCDB3}, // 统
  {0x521D, 0xB3F5}, // 初
  {0x59CB, 0xCABC}, // 始
  {0x5316, 0xBBAF}, // 化
  {0x5B8C, 0xCDEA}, // 完
  {0x6BD5, 0xB1D2}, // 毕
  {0x4E00, 0xD2BB}, // 一
  {0x4E8C, 0xB6FE}, // 二
  {0x4E09, 0xC8FD}, // 三
  {0x56DB, 0xCBC4}, // 四
  {0x4E94, 0xCEE5}, // 五
  {0x516D, 0xC1F9}, // 六
  {0x4E03, 0xC6DF}, // 七
  {0x516B, 0xB0CB}, // 八
  // 可以根据需要添加更多常用字
};

// UTF-8 转 GB2312
inline uint16_t utf8ToGb2312(uint32_t utf8) {
  for (size_t i = 0; i < sizeof(utf8_to_gb2312) / sizeof(utf8_to_gb2312[0]); i++) {
    if (utf8_to_gb2312[i].utf8 == utf8) {
      return utf8_to_gb2312[i].gb2312;
    }
  }
  return 0; // 未找到
}

// 计算 ASCII 字模偏移量
// 参数：
//   c - ASCII 字符
// 返回：
//   字模在芯片中的起始地址
inline uint32_t getASCIIOffset(char c) {
  return ASCII_8x16_START + ((uint8_t)c) * ASCII_8x16_BYTES;
}

// 计算中文字模偏移量
// 参数：
//   area - GB2312 区码（0xA1-0xFE）
//   index - GB2312 位码（0xA1-0xFE）
// 返回：
//   字模在芯片中的起始地址
inline uint32_t getHZ16Offset(uint8_t area, uint8_t index) {
  // 正确的计算公式：起始地址 + ((区码 - 0xA1) * 94 + (位码 - 0xA1)) * 每字符字节数
  return GB2312_16x16_START + (((area - 0xA1) * 94) + (index - 0xA1)) * GB2312_16x16_BYTES;
}

// 根据 UTF-8 字符计算中文字模偏移量
// 参数：
//   utf8 - UTF-8 码点
// 返回：
//   字模在芯片中的起始地址，未找到返回 0
inline uint32_t getHZ16OffsetFromUtf8(uint32_t utf8) {
  uint16_t gb2312 = utf8ToGb2312(utf8);
  if (gb2312 == 0) return 0;
  
  uint8_t area = (gb2312 >> 8) & 0xFF;
  uint8_t index = gb2312 & 0xFF;
  
  // 验证区码和位码是否在有效范围内
  if (area < 0xA1 || area > 0xFE || index < 0xA1 || index > 0xFE) {
    return 0;
  }
  
  return getHZ16Offset(area, index);
}

// 设置字体起始地址和字节数（从字库目录读取后调用）
// 参数：
//   fontType - 字体类型（0=ASCII，1=中文）
//   width - 字体宽度
//   height - 字体高度
//   startAddr - 起始地址
//   bytesPerChar - 每字符字节数
inline void setFontInfo(uint8_t fontType, uint16_t width, uint16_t height, uint32_t startAddr, uint8_t bytesPerChar) {
  if (fontType == 0) { // ASCII 字体
    if (width == 8 && height == 16) {
      ASCII_8x16_START = startAddr;
      ASCII_8x16_BYTES = bytesPerChar;
    }
  } else { // 中文字体
    if (width == 16 && height == 16) {
      GB2312_16x16_START = startAddr;
      GB2312_16x16_BYTES = bytesPerChar;
    }
  }
}

#endif // FONT_INDEX_H
