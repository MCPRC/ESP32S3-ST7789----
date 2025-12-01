#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

// 通用类型定义

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;

typedef float    f32;
typedef double   f64;

// 颜色类型
typedef uint16_t Color;

// 坐标类型
typedef struct {
  int x;
  int y;
} Point;

typedef struct {
  int x;
  int y;
  int width;
  int height;
} Rect;

#endif // TYPES_H
