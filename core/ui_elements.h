#ifndef UI_ELEMENTS_H
#define UI_ELEMENTS_H

#include <stdint.h>
#include "draw_util.h"
#include "font_render.h"

class UIElement {
public:
  virtual void draw(uint16_t *fb) = 0;
  virtual void update() = 0;
  
  void setPosition(int x, int y) {
    _x = x;
    _y = y;
  }
  
protected:
  int _x;
  int _y;
};

class UILabel : public UIElement {
public:
  UILabel(int x, int y, const char* text, uint16_t color);
  void draw(uint16_t *fb) override;
  void update() override;
  void setText(const char* text);
  
private:
  const char* _text;
  uint16_t _color;
};

class UIButton : public UIElement {
public:
  UIButton(int x, int y, int width, int height, const char* text, uint16_t bgColor, uint16_t textColor);
  void draw(uint16_t *fb) override;
  void update() override;
  bool isPressed(int x, int y);
  
private:
  int _width;
  int _height;
  const char* _text;
  uint16_t _bgColor;
  uint16_t _textColor;
};

#endif // UI_ELEMENTS_H
