#ifndef YATO_DEVICE_VIDEO_CONSOLE_CONSOLE_H
#define YATO_DEVICE_VIDEO_CONSOLE_CONSOLE_H

#include <stdint.h>

#include "lib/string/string.h"

typedef uint8_t VGAConsole_Color;
typedef uint16_t VGAConsole_Pixel;

// Top left corner of screen is considered (0, 0)
// Bottom right corner of screen is considered (x, y)
struct VGAConsole {
  int cursor_x;
  int cursor_y;
  int screen_width;
  int screen_height;
  int default_color;
  VGAConsole_Pixel *frame_buffer;
};

enum VGAColorCode {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
};

static const uint32_t kVgaConsoleDefaultWidth = 80;
static const uint32_t kVgaConsoleDefaultHeight = 25;
static const uint16_t *kVgaConsoleDefaultFBLocation = (uint16_t *)0xB8000;

inline VGAConsole_Color VGAConsole_EncodeColor(enum VGAColorCode fg,
                                               enum VGAColorCode bg) {
  return fg | bg << 4;
}

inline VGAConsole_Pixel VGAConsole_EncodePixel(unsigned char c,
                                               VGAConsole_Color color) {
  return (VGAConsole_Pixel)c | (VGAConsole_Pixel)color << 8;
}

inline uint32_t VGAConsole_DimensionToIndex(struct VGAConsole *console,
                                            uint32_t x, uint32_t y) {
  return y * console->screen_width + x;
}

void VGAConsole_Initialize(struct VGAConsole *console, int width, int height,
                           uint16_t *buffer, enum VGAColorCode fg,
                           enum VGAColorCode bg);

void VGAConsole_WriteChar(struct VGAConsole *console, char c);

int VGAConsole_WriteN(struct VGAConsole *console, const char *data,
                      uint32_t len);

int VGAConsole_Write(struct VGAConsole *console, const char *data);

void VGAConsole_ClearScreen(struct VGAConsole *console);

#endif  // YATO_DEVICE_VIDEO_CONSOLE_CONSOLE_H
