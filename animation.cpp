#include "core/animation.h"
#include <Arduino.h>

// Animation 基类实现
Animation::Animation() {
  _startTime = 0;
  _duration = 0;
  _isRunning = false;
}

// FadeAnimation 实现
FadeAnimation::FadeAnimation(int x, int y, int width, int height, uint16_t startColor, uint16_t endColor, uint32_t duration) {
  _x = x;
  _y = y;
  _width = width;
  _height = height;
  _startColor = startColor;
  _endColor = endColor;
  _currentColor = startColor;
  _duration = duration;
  _startTime = millis();
  _isRunning = true;
}

void FadeAnimation::update() {
  if (!_isRunning) return;
  
  uint32_t elapsed = millis() - _startTime;
  if (elapsed >= _duration) {
    _currentColor = _endColor;
    _isRunning = false;
    return;
  }
  
  float progress = (float)elapsed / _duration;
  
  // 分解颜色分量
  uint8_t startR = (_startColor >> 11) & 0x1F;
  uint8_t startG = (_startColor >> 5) & 0x3F;
  uint8_t startB = _startColor & 0x1F;
  
  uint8_t endR = (_endColor >> 11) & 0x1F;
  uint8_t endG = (_endColor >> 5) & 0x3F;
  uint8_t endB = _endColor & 0x1F;
  
  // 计算当前颜色分量
  uint8_t currentR = startR + (endR - startR) * progress;
  uint8_t currentG = startG + (endG - startG) * progress;
  uint8_t currentB = startB + (endB - startB) * progress;
  
  // 重新组合为16位颜色
  _currentColor = (currentR << 11) | (currentG << 5) | currentB;
}

void FadeAnimation::draw(uint16_t *fb) {
  DrawUtil::fillRect(fb, _x, _y, _width, _height, _currentColor);
}

bool FadeAnimation::isFinished() {
  return !_isRunning;
}

// SlideAnimation 实现
SlideAnimation::SlideAnimation(int startX, int startY, int endX, int endY, int width, int height, uint16_t color, uint32_t duration) {
  _startX = startX;
  _startY = startY;
  _endX = endX;
  _endY = endY;
  _currentX = startX;
  _currentY = startY;
  _width = width;
  _height = height;
  _color = color;
  _duration = duration;
  _startTime = millis();
  _isRunning = true;
}

void SlideAnimation::update() {
  if (!_isRunning) return;
  
  uint32_t elapsed = millis() - _startTime;
  if (elapsed >= _duration) {
    _currentX = _endX;
    _currentY = _endY;
    _isRunning = false;
    return;
  }
  
  float progress = (float)elapsed / _duration;
  
  _currentX = _startX + (_endX - _startX) * progress;
  _currentY = _startY + (_endY - _startY) * progress;
}

void SlideAnimation::draw(uint16_t *fb) {
  DrawUtil::fillRect(fb, _currentX, _currentY, _width, _height, _color);
}

bool SlideAnimation::isFinished() {
  return !_isRunning;
}
