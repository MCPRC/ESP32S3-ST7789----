#include "core/ui_elements.h"

// UILabel 实现
UILabel::UILabel(int x, int y, const char* text, uint16_t color) {
  _x = x;
  _y = y;
  _text = text;
  _color = color;
}

void UILabel::draw(uint16_t *fb) {
  FontRender::drawString(fb, _x, _y, _text, _color);
}

void UILabel::update() {
  // 标签不需要更新
}

void UILabel::setText(const char* text) {
  _text = text;
}

// UIButton 实现
UIButton::UIButton(int x, int y, int width, int height, const char* text, uint16_t bgColor, uint16_t textColor) {
  _x = x;
  _y = y;
  _width = width;
  _height = height;
  _text = text;
  _bgColor = bgColor;
  _textColor = textColor;
}

void UIButton::draw(uint16_t *fb) {
  // 绘制按钮背景
  DrawUtil::fillRect(fb, _x, _y, _width, _height, _bgColor);
  
  // 绘制按钮边框
  DrawUtil::drawRect(fb, _x, _y, _width, _height, _textColor);
  
  // 绘制按钮文本
  int textX = _x + (_width - strlen(_text) * 8) / 2;
  int textY = _y + (_height - 16) / 2;
  FontRender::drawString(fb, textX, textY, _text, _textColor);
}

void UIButton::update() {
  // 按钮不需要更新
}

bool UIButton::isPressed(int x, int y) {
  return (x >= _x && x <= _x + _width && y >= _y && y <= _y + _height);
}
