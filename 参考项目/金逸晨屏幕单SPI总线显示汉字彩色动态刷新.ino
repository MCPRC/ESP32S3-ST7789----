#include <SPI.h>

// ===== SPI 总线（共用）=====
#define PIN_SCK   10
#define PIN_MOSI  9
#define PIN_MISO  2
SPIClass spi1(1);

// ===== 屏幕 1 =====
#define LCD1_CS   6
#define LCD1_DC   8
#define LCD1_RST  7
#define LCD1_BL   11

// ===== 屏幕 2 =====
#define LCD2_CS   5
#define LCD2_DC   13
#define LCD2_RST  12
#define LCD2_BL   1

// ===== 字库 Flash =====
#define FLASH_CS  4

// ===== 字库配置 =====
const uint32_t FONT16_START = 0x71300;
const uint8_t FONT16_BYTES  = 32;

// ASCII 8×16 字库（用于 FPS）
const uint32_t ASCII_8x16_START = 0x1D00;
const uint8_t ASCII_8x16_BYTES  = 16;

// ===== DMA 帧缓存 =====
uint16_t *fb1;
uint16_t *fb2;

// ===== 中文字模缓存 =====
uint8_t hz_YI[32];
uint8_t hz_ER[32];
uint8_t hz_SAN[32];
uint8_t hz_SI[32];

uint8_t hz_WU[32];
uint8_t hz_LIU[32];
uint8_t hz_QI[32];
uint8_t hz_BA[32];

// ===== 字模地址函数 =====
uint32_t getHZ16Offset(uint8_t area, uint8_t index) {
  return FONT16_START + (((area - 0xA1) * 94) + (index - 0xA1)) * FONT16_BYTES;
}

uint32_t getASCIIOffset(char c) {
  return ASCII_8x16_START + ((uint8_t)c) * ASCII_8x16_BYTES;
}

// ===== Flash 读取 =====
void flashRead(uint32_t addr, uint8_t* buf, uint8_t len = 32) {
  digitalWrite(LCD1_CS, HIGH);
  digitalWrite(LCD2_CS, HIGH);
  digitalWrite(FLASH_CS, HIGH);
  digitalWrite(FLASH_CS, LOW);

  spi1.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  spi1.transfer(0x03);
  spi1.transfer(addr >> 16);
  spi1.transfer(addr >> 8);
  spi1.transfer(addr);

  for (int i = 0; i < len; i++)
    buf[i] = spi1.transfer(0x00);

  spi1.endTransaction();
  digitalWrite(FLASH_CS, HIGH);
}

// =============================================================
// ST7789 基础函数
// =============================================================
void writeCmd(int cs, int dc, uint8_t cmd) {
  digitalWrite(LCD1_CS, HIGH);
  digitalWrite(LCD2_CS, HIGH);
  digitalWrite(FLASH_CS, HIGH);

  digitalWrite(cs, LOW);
  digitalWrite(dc, LOW);
  spi1.transfer(cmd);
  digitalWrite(cs, HIGH);
}

void writeData(int cs, int dc, uint8_t data) {
  digitalWrite(LCD1_CS, HIGH);
  digitalWrite(LCD2_CS, HIGH);
  digitalWrite(FLASH_CS, HIGH);

  digitalWrite(cs, LOW);
  digitalWrite(dc, HIGH);
  spi1.transfer(data);
  digitalWrite(cs, HIGH);
}

// =============================================================
// ST7789 初始化
// =============================================================
void lcdInit(int cs, int dc, int rst) {
  pinMode(cs, OUTPUT);
  pinMode(dc, OUTPUT);
  pinMode(rst, OUTPUT);

  digitalWrite(rst, LOW); delay(80);
  digitalWrite(rst, HIGH); delay(120);

  writeCmd(cs, dc, 0x11);
  delay(120);

  writeCmd(cs, dc, 0x36); writeData(cs, dc, 0x00);
  writeCmd(cs, dc, 0x3A); writeData(cs, dc, 0x55);

  writeCmd(cs, dc, 0x21);
  writeCmd(cs, dc, 0x29);
}

// =============================================================
// DMA 显示函数
// =============================================================
void setFullAddr(int cs, int dc) {
  writeCmd(cs, dc, 0x2A);
  writeData(cs, dc, 0);
  writeData(cs, dc, 0);
  writeData(cs, dc, 0);
  writeData(cs, dc, 239);

  writeCmd(cs, dc, 0x2B);
  writeData(cs, dc, 0);
  writeData(cs, dc, 0);
  writeData(cs, dc, 0);
  writeData(cs, dc, 239);

  writeCmd(cs, dc, 0x2C);
}

void dmaPush(int cs, int dc, uint16_t *fb) {
  setFullAddr(cs, dc);

  digitalWrite(LCD1_CS, HIGH);
  digitalWrite(LCD2_CS, HIGH);
  digitalWrite(FLASH_CS, HIGH);

  digitalWrite(cs, LOW);
  digitalWrite(dc, HIGH);

  spi1.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));
  spi1.writePixels((uint8_t*)fb, 240 * 240 * 2);
  spi1.endTransaction();

  digitalWrite(cs, HIGH);
}

// =============================================================
// 绘图：清屏
// =============================================================
void clearFB(uint16_t *fb, uint16_t color) {
  for (int i = 0; i < 240*240; i++)
    fb[i] = color;
}

// =============================================================
// 绘图：中文 16×16
// =============================================================
void drawHZ16_FB(uint16_t *fb, int x, int y, uint8_t *font, uint16_t color) {
  for (int row = 0; row < 16; row++) {
    uint16_t line = (font[row*2] << 8) | font[row*2+1];
    for (int bit = 0; bit < 16; bit++) {
      if (line & (0x8000 >> bit))
        fb[(y+row)*240 + (x+bit)] = color;
    }
  }
}

// =============================================================
// 绘图：ASCII（用于 FPS）
// =============================================================
void drawASCII_FB(uint16_t *fb, int x, int y, char c, uint16_t color) {
  uint8_t font[16];
  flashRead(getASCIIOffset(c), font, 16);

  for (int row = 0; row < 16; row++) {
    uint8_t line = font[row];
    for (int bit = 0; bit < 8; bit++) {
      if (line & (0x80 >> bit))
        fb[(y+row)*240 + (x+bit)] = color;
    }
  }
}

void drawString_FB(uint16_t *fb, int x, int y, const char* str, uint16_t color) {
  while (*str) {
    drawASCII_FB(fb, x, y, *str, color);
    x += 8;
    str++;
  }
}

// =============================================================
// 动态颜色
// =============================================================
uint16_t wheelColor(uint16_t t) {
  uint8_t r = (sin(t * 0.05) * 127) + 128;
  uint8_t g = (sin(t * 0.08 + 2) * 127) + 128;
  uint8_t b = (sin(t * 0.13 + 4) * 127) + 128;
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// =============================================================
// SETUP：初始化与加载字形
// =============================================================
void setup() {
  spi1.begin(PIN_SCK, PIN_MISO, PIN_MOSI);

  pinMode(LCD1_BL, OUTPUT);
  pinMode(LCD2_BL, OUTPUT);
  digitalWrite(LCD1_BL, HIGH);
  digitalWrite(LCD2_BL, HIGH);

  pinMode(FLASH_CS, OUTPUT);
  digitalWrite(FLASH_CS, HIGH);

  lcdInit(LCD1_CS, LCD1_DC, LCD1_RST);
  lcdInit(LCD2_CS, LCD2_DC, LCD2_RST);

  fb1 = (uint16_t*)heap_caps_malloc(240*240*2, MALLOC_CAP_DMA);
  fb2 = (uint16_t*)heap_caps_malloc(240*240*2, MALLOC_CAP_DMA);

  clearFB(fb1, 0x001F);   // 蓝背景
  clearFB(fb2, 0xF800);   // 红背景

  // 读取 “一 二 三 四”
  flashRead(getHZ16Offset(0xD2, 0xBB), hz_YI);
  flashRead(getHZ16Offset(0xB6, 0xFE), hz_ER);
  flashRead(getHZ16Offset(0xC8, 0xFD), hz_SAN);
  flashRead(getHZ16Offset(0xCB, 0xC4), hz_SI);

  // 读取 “五 六 七 八”
  flashRead(getHZ16Offset(0xCE, 0xE5), hz_WU);
  flashRead(getHZ16Offset(0xC1, 0xF9), hz_LIU);
  flashRead(getHZ16Offset(0xC6, 0xDF), hz_QI);
  flashRead(getHZ16Offset(0xB0, 0xCB), hz_BA);
}

// =============================================================
// LOOP：动态颜色 + FPS + 双屏刷新
// =============================================================
void loop() {
  static uint32_t fps1 = 0, fps2 = 0;
  static uint32_t frame1 = 0, frame2 = 0;
  static uint32_t last = 0;

  uint32_t now = millis();
  frame1++; frame2++;

  if (now - last >= 1000) {
    fps1 = frame1;
    fps2 = frame2;
    frame1 = frame2 = 0;
    last = now;
  }

  static uint16_t t = 0;
  t++;

  // 清屏
  clearFB(fb1, 0x001F);
  clearFB(fb2, 0xF800);

  // 屏幕 1：一二三四
  drawHZ16_FB(fb1, 10, 40, hz_YI,  wheelColor(t));
  drawHZ16_FB(fb1, 30, 40, hz_ER,  wheelColor(t+20));
  drawHZ16_FB(fb1, 50, 40, hz_SAN, wheelColor(t+40));
  drawHZ16_FB(fb1, 70, 40, hz_SI,  wheelColor(t+60));

  // 屏幕 2：五六七八
  drawHZ16_FB(fb2, 10, 40, hz_WU,  wheelColor(t));
  drawHZ16_FB(fb2, 30, 40, hz_LIU, wheelColor(t+20));
  drawHZ16_FB(fb2, 50, 40, hz_QI,  wheelColor(t+40));
  drawHZ16_FB(fb2, 70, 40, hz_BA,  wheelColor(t+60));

  // FPS
  char buf1[16], buf2[16];
  sprintf(buf1, "FPS:%lu", fps1);
  sprintf(buf2, "FPS:%lu", fps2);

  drawString_FB(fb1, 5, 5, buf1, 0xFFFF);
  drawString_FB(fb2, 5, 5, buf2, 0xFFFF);

  // DMA 输出
  dmaPush(LCD1_CS, LCD1_DC, fb1);
  dmaPush(LCD2_CS, LCD2_DC, fb2);
}
