#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdint.h>
#include "draw_util.h"

class Animation {
public:
  Animation();
  virtual void update() = 0;
  virtual void draw(uint16_t *fb) = 0;
  virtual bool isFinished() = 0;
  
protected:
  uint32_t _startTime;
  uint32_t _duration;
  bool _isRunning;
};

class FadeAnimation : public Animation {
public:
  FadeAnimation(int x, int y, int width, int height, uint16_t startColor, uint16_t endColor, uint32_t duration);
  void update() override;
  void draw(uint16_t *fb) override;
  bool isFinished() override;
  
private:
  int _x;
  int _y;
  int _width;
  int _height;
  uint16_t _startColor;
  uint16_t _endColor;
  uint16_t _currentColor;
};

class SlideAnimation : public Animation {
public:
  SlideAnimation(int startX, int startY, int endX, int endY, int width, int height, uint16_t color, uint32_t duration);
  void update() override;
  void draw(uint16_t *fb) override;
  bool isFinished() override;
  
private:
  int _startX;
  int _startY;
  int _endX;
  int _endY;
  int _currentX;
  int _currentY;
  int _width;
  int _height;
  uint16_t _color;
};

#endif // ANIMATION_H
