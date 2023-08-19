#include "arch/x86_64/boot/console.h"
#include "lib/string/string.h"

const uint32_t kVgaConsoleDefaultWidth = 80;
const uint32_t kVgaConsoleDefaultHeight = 25;
// TODO: this hardcoded value only works if in protected mode once handed off
// GNU Grub 2. Maybe we make this a configurable option if we ever support
// multiple bootloaders
const VGAConsole_Pixel *kVgaConsoleDefaultFBLocation =
    (VGAConsole_Pixel *)0xB8000;

void VGAConsole_WriteChar(VGAConsole *console, char c) {
  const uint32_t i = VGAConsole_DimensionToIndex(console, console->cursor_x,
                                                 console->cursor_y);
  switch (c) {
    case '\r':
    case '\n':
      goto newline;
      break;
    default:
      console->frame_buffer[i] =
          VGAConsole_EncodePixel(c, console->default_color);
      if (++console->cursor_x == console->screen_width) {
      newline:
        console->cursor_x = 0;
        if (++console->cursor_y == console->screen_height) {
          console->cursor_y = 0;
        }
      }
  }
}

void VGAConsole_ClearScreen(VGAConsole *console) {
  for (int y = 0; y < console->screen_height; ++y) {
    for (int x = 0; x < console->screen_width; ++x) {
      const uint32_t i = VGAConsole_DimensionToIndex(console, x, y);
      console->frame_buffer[i] =
          VGAConsole_EncodePixel(' ', console->default_color);
    }
  }
}

void VGAConsole_Initialize(VGAConsole *console, int width, int height,
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

static void IntegerToArg(char *buf, int base, int d) {
  char *p = buf;
  unsigned long ud = d;
  int divisor = 10;
  if (base == 'd' && d < 0) {
    *p++ = '-';
    buf++;
    ud = -d;
  } else if (base == 'x')
    divisor = 16;

  do {
    int r = ud % divisor;
    *p++ = (r < 10) ? r + '0' : r + 'a' - 10;
  } while (ud /= divisor);
  *p = 0;
  char *p1 = buf;
  char *p2 = p - 1;
  while (p1 < p2) {
    char tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    p1++;
    p2--;
  }
}

int VGAConsole_WriteF(VGAConsole *console, const char *format, ...) {
  char **arg = (char **)&format;
  int c;
  char buf[20];
  arg++;
  while ((c = *format++) != 0) {
    if (c != '%')
      VGAConsole_WriteChar(console, c);
    else {
      char *p, *p2;
      int pad0 = 0, pad = 0;
      c = *format++;
      if (c == '0') {
        pad0 = 1;
        c = *format++;
      }
      if (c >= '0' && c <= '9') {
        pad = c - '0';
        c = *format++;
      }
      switch (c) {
        case 'd':
        case 'u':
        case 'x':
          IntegerToArg(buf, c, *((int *)arg++));
          p = buf;
          goto string;
          break;
        case 's':
          p = *arg++;
          if (!p) p = "(null)";
        string:
          for (p2 = p; *p2; p2++) {
          }
          for (; p2 < p + pad; p2++)
            VGAConsole_WriteChar(console, pad0 ? '0' : ' ');
          while (*p) VGAConsole_WriteChar(console, *p++);
          break;
        default:
          VGAConsole_WriteChar(console, *((int *)arg++));
          break;
      }
    }
  }
}

int VGAConsole_WriteN(VGAConsole *console, const char *data, uint32_t len) {
  for (uint32_t i = 0; i < len; ++i) VGAConsole_WriteChar(console, data[i]);
  return len;
}

int VGAConsole_Write(VGAConsole *console, const char *data) {
  return VGAConsole_WriteN(console, data, strlen(data));
}
