#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// 包含项目头文件
#include "pins/bus_pins.h"
#include "drivers/spi_wrapper.h"
#include "drivers/font_driver.h"
#include "core/display_manager.h"
#include "protocol/command_parser.h"
#include "protocol/command_executor.h"
#include "util/settings.h"

// 应用头文件
#include "apps/demo01/demo01.h"
#include "apps/demo02/demo02.h"
#include "apps/demo03/demo03.h"
#include "apps/demo04/demo04.h"
#include "apps/demo05/demo05.h"
#include "apps/demo06/demo06.h"
#include "apps/demo07/demo07.h"

// 应用注册函数声明
void register_demo01();
void register_demo02();
void register_demo03();
void register_demo04();
void register_demo05();
void register_demo06();
void register_demo07();
// ESP-NOW 相关
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// 串口缓冲区
uint8_t serialBuffer[256];
int serialBufferIndex = 0;

// 系统状态
bool isInitialized = false;

// ESP-NOW 接收回调函数
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  Packet packet;
  if (CommandParser::parsePacket((uint8_t*)incomingData, len, packet)) {
    CommandExecutor::executeCommand(packet);
  }
}

// 初始化 ESP-NOW
void initESPNOW() {
  // 设置 WiFi 模式为 STA
  WiFi.mode(WIFI_STA);
  
  // 初始化 ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // 注册接收回调
  esp_now_register_recv_cb(OnDataRecv);
  
  Serial.println("ESP-NOW initialized");
}

// 初始化串口
void initSerial() {
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }
  Serial.println("Serial initialized");
}

// ASCII字符转十六进制值
uint8_t hexCharToByte(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  return 0;
}

// 处理串口数据
void processSerialData() {
  static String serialStr = "";
  
  while (Serial.available()) {
    char c = Serial.read();
    
    // 打印接收到的字符
    Serial.printf("Received char: '%c' (0x%02X)\n", c, (uint8_t)c);
    
    // 如果是换行符，处理完整的字符串
    if (c == '\n' || c == '\r') {
      if (serialStr.length() > 0) {
        Serial.printf("Processing string: \"%s\"\n", serialStr.c_str());
        
        // 检查是否是APP命令格式
        if (serialStr.startsWith("APP ")) {
          // 提取应用名称
          String appName = serialStr.substring(4);
          Serial.printf("APP command detected, appName: %s\n", appName.c_str());
          
          // 根据应用名称启动对应的应用
          if (appName.equals("demo01")) {
            Serial.println("Starting demo01...");
            AppManager::runApp(0x01);
          } else if (appName.equals("demo02")) {
            Serial.println("Starting demo02...");
            AppManager::runApp(0x02);
          } else if (appName.equals("demo03")) {
            Serial.println("Starting demo03...");
            AppManager::runApp(0x03);
          } else if (appName.equals("demo04")) {
            Serial.println("Starting demo04...");
            AppManager::runApp(0x04);
          } else if (appName.equals("demo05")) {
            Serial.println("Starting demo05...");
            AppManager::runApp(0x05);
          } else if (appName.equals("demo06")) {
            Serial.println("Starting demo06...");
            AppManager::runApp(0x06);
          } else if (appName.equals("demo07")) {
            Serial.println("Starting demo07...");
            AppManager::runApp(0x07);
          } else {
            Serial.printf("Unknown app name: %s\n", appName.c_str());
          }
        } else {
          // 原有逻辑：解析ASCII字符串为二进制数据
          int binaryIndex = 0;
          uint8_t binaryBuffer[256] = {0};
          
          for (int i = 0; i < serialStr.length(); i += 3) {
            // 跳过空格
            while (i < serialStr.length() && serialStr[i] == ' ') i++;
            if (i + 1 >= serialStr.length()) break;
            
            // 解析两个十六进制字符为一个字节
            uint8_t high = hexCharToByte(serialStr[i]);
            uint8_t low = hexCharToByte(serialStr[i+1]);
            uint8_t byteValue = (high << 4) | low;
            binaryBuffer[binaryIndex++] = byteValue;
            
            Serial.printf("Parsed byte: 0x%02X\n", byteValue);
          }
          
          // 尝试解析二进制数据包
          Packet packet;
          if (CommandParser::parsePacket(binaryBuffer, binaryIndex, packet)) {
            Serial.println("Valid packet received!");
            Serial.printf("Packet command: 0x%02X, data length: %d\n", packet.command, packet.dataLength);
            Serial.print("Packet data: ");
            for (int i = 0; i < packet.dataLength; i++) {
              Serial.printf("0x%02X ", packet.data[i]);
            }
            Serial.println();
            
            CommandExecutor::executeCommand(packet);
          }
        }
        
        // 清空字符串
        serialStr = "";
      }
    } else {
      // 否则，将字符添加到字符串中
      serialStr += c;
    }
  }
}

void setup() {
  // 初始化串口
  initSerial();
  
  // 初始化SPI
  SPIWrapper::init();
  
  // 初始化字库驱动
  FontDriver::init();
  
  // 初始化显示管理器
  DisplayManager::init();
  
  // 初始化设置
  Settings::init();
  
  // 初始化 ESP-NOW
  initESPNOW();
  
  // 标记系统已初始化
  isInitialized = true;
  
  Serial.println("System initialized");
  
  // 手动注册应用
  register_demo01();
  register_demo02();
  register_demo03();
  register_demo04();
  register_demo05();
  register_demo06();
  register_demo07();
  
  // 自动运行 demo01，使用任务方式
  AppManager::runApp(0x01);
}

void loop() {
  // 处理串口数据
  processSerialData();
  
  // 主循环中可以添加其他任务
  delay(10);
}
