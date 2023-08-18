#include "arch/x86_64/boot/console.h"
#include "lib/string/string.h"

void VGAConsole_WriteChar(struct VGAConsole *console, char c) {
  const uint32_t i = VGAConsole_DimensionToIndex(console, console->cursor_x,
                                                 console->cursor_y);
  console->frame_buffer[i] = VGAConsole_EncodePixel(c, console->default_color);
  if (++console->cursor_x == console->screen_width) {
    console->cursor_x = 0;
    if (++console->cursor_y == console->screen_height) {
      console->cursor_y = 0;
    }
  }
}

void VGAConsole_ClearScreen(struct VGAConsole *console) {
  for (int y = 0; y < console->screen_height; ++y) {
    for (int x = 0; x < console->screen_width; ++x) {
      const uint32_t i = VGAConsole_DimensionToIndex(console, x, y);
      console->frame_buffer[i] =
          VGAConsole_EncodePixel(' ', console->default_color);
    }
  }
}

void VGAConsole_Initialize(struct VGAConsole *console, int width, int height,
                           uint16_t *buffer, enum VGAColorCode fg,
                           enum VGAColorCode bg) {
  console->cursor_x = 0;
  console->cursor_y = 0;
  console->screen_width = width;
  console->screen_height = height;
  console->default_color = VGAConsole_EncodeColor(fg, bg);
  console->frame_buffer = buffer;
  VGAConsole_ClearScreen(console);
}

int VGAConsole_WriteN(struct VGAConsole *console, const char *data,
                      uint32_t len) {
  for (uint32_t i = 0; i < len; ++i) {
    VGAConsole_WriteChar(console, data[i]);
  }
  return len;
}

int VGAConsole_Write(struct VGAConsole *console, const char *data) {
  return VGAConsole_WriteN(console, data, strlen(data));
}
