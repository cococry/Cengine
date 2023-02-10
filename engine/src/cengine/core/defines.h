#pragma once

#define false 0
#define true 1
#define nullptr (void*)0

#define RGB_COLOR(r, g, b) r / 255.0f, g / 255.0f, b / 255.0f
#define RGBA_COLOR(r, g, b, a) r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f

#define INVALID_UV vector2_create(-1.0f, -1.0f)
#define INVALID_COLOR vector4_create(-1.0f, -1.0f, -1.0f, -1.0f)

typedef unsigned char bool8;
typedef unsigned int bool32;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;