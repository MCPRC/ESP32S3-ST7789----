#include <SPI.h>
#include "output.c"     // 240*240/8 = 7200 字节纯 1bit 图像

// ---------------- SPI 引脚 ----------------
#define PIN_SCK   10
#define PIN_MOSI   9
#define PIN_MISO   2

// ---------------- 屏幕 1 引脚 ----------------
#define CS1    6
#define DC1    8
#define RST1   7
#define BL1   11

// ---------------- 屏幕 2 引脚 ----------------
#define CS2    5
#define DC2   13
#define RST2  12
#define BL2    1

SPIClass spi(1);   // SPI1，可映射 GPIO

// =========================================================
//                 LCD 基础指令
// =========================================================
void lcd_cmd(uint8_t cs, uint8_t dc, uint8_t cmd)
{
    digitalWrite(dc, LOW);
    digitalWrite(cs, LOW);
    spi.transfer(cmd);
    digitalWrite(cs, HIGH);
}

void lcd_data(uint8_t cs, uint8_t dc, uint8_t data)
{
    digitalWrite(dc, HIGH);
    digitalWrite(cs, LOW);
    spi.transfer(data);
    digitalWrite(cs, HIGH);
}

// =========================================================
//                 LCD 设置窗口
// =========================================================
void lcd_set_window(uint8_t cs, uint8_t dc,
                    int x0, int y0, int x1, int y1)
{
    lcd_cmd(cs, dc, 0x2A);
    lcd_data(cs, dc, x0 >> 8);
    lcd_data(cs, dc, x0 & 0xFF);
    lcd_data(cs, dc, x1 >> 8);
    lcd_data(cs, dc, x1 & 0xFF);

    lcd_cmd(cs, dc, 0x2B);
    lcd_data(cs, dc, y0 >> 8);
    lcd_data(cs, dc, y0 & 0xFF);
    lcd_data(cs, dc, y1 >> 8);
    lcd_data(cs, dc, y1 & 0xFF);
}

// =========================================================
//   ★★★ ST7789 初始化（RGB + BigEndian + inversion ON）
// =========================================================
void st7789_init(uint8_t cs, uint8_t dc, uint8_t rst)
{
    pinMode(cs, OUTPUT);
    pinMode(dc, OUTPUT);
    pinMode(rst, OUTPUT);

    digitalWrite(cs, HIGH);

    digitalWrite(rst, LOW);
    delay(50);
    digitalWrite(rst, HIGH);
    delay(150);

    // MADCTL：RGB，无翻转，无镜像
    lcd_cmd(cs, dc, 0x36);
    lcd_data(cs, dc, 0x00);

    // 16bit RGB565
    lcd_cmd(cs, dc, 0x3A);
    lcd_data(cs, dc, 0x05);

    // 必须 inversion ON，否则黑白颠倒
    lcd_cmd(cs, dc, 0x21);
    delay(10);

    lcd_cmd(cs, dc, 0x11);
    delay(120);

    lcd_cmd(cs, dc, 0x29);
    delay(20);
}

// =========================================================
//     ★★★ 1bit → RGB565（Big Endian: MSB → LSB）
// =========================================================
uint8_t lineBuffer[240 * 2];

void convert1bitToRGB565_BE(
    const uint8_t* src,
    uint8_t* dst,
    uint16_t color0,
    uint16_t color1)
{
    for (int x = 0; x < 240; x++)
    {
        uint8_t byte = src[x >> 3];
        uint8_t bit  = 0x80 >> (x & 7);

        // 选择 RGB565 颜色
        uint16_t c = (byte & bit) ? color1 : color0;

        // Big Endian（高字节 → 低字节）
        dst[x * 2 + 0] = c >> 8;    // MSB
        dst[x * 2 + 1] = c & 0xFF;  // LSB
    }
}

// =========================================================
//                 显示整张图像
// =========================================================
void displayImage1bit(uint8_t cs, uint8_t dc,
                      uint16_t color0, uint16_t color1)
{
    lcd_set_window(cs, dc, 0, 0, 239, 239);
    lcd_cmd(cs, dc, 0x2C); // Memory Write

    spi.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));

    digitalWrite(cs, LOW);
    digitalWrite(dc, HIGH);

    const int BPL = 240 / 8;

    for (int y = 0; y < 240; y++)
    {
        const uint8_t *src = image_1bit + y * BPL;

        convert1bitToRGB565_BE(src, lineBuffer, color0, color1);

        spi.writeBytes(lineBuffer, 240 * 2);  // DMA（无 byte-swap）
    }

    digitalWrite(cs, HIGH);

    spi.endTransaction();
}

// =========================================================
//                       setup
// =========================================================
void setup()
{
    Serial.begin(115200);

    spi.begin(PIN_SCK, PIN_MISO, PIN_MOSI, -1);

    // 背光打开
    pinMode(BL1, OUTPUT); digitalWrite(BL1, HIGH);
    pinMode(BL2, OUTPUT); digitalWrite(BL2, HIGH);

    // 初始化两块屏幕
    st7789_init(CS1, DC1, RST1);
    st7789_init(CS2, DC2, RST2);

    // 测试颜色：红色
    uint16_t color0 = 0x0000;   // 黑色
    uint16_t color1 = 0xF800;   // 红（RGB565）

    // 同步显示到两个屏幕
    displayImage1bit(CS1, DC1, color0, color1);
    displayImage1bit(CS2, DC2, color0, color1);

    Serial.println("双屏显示完成");
}

void loop()
{
}
