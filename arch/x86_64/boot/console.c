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
  int count = 0;
  char **arg = (char **)&format;
  arg++;
  char format_iter;
  char buffer[20];
  while ((format_iter = *format++) != 0) {
    if (format_iter != '%') {
      count += 1;
      VGAConsole_WriteChar(console, format_iter);
    } else {
      char *buffer_iter;
      char *buffer_end;
      int zero_padding = 0;
      int nonzero_padding = 0;
      format_iter = *format++;
      if (format_iter == '0') {
        zero_padding = 1;
        format_iter = *format++;
      }
      if (format_iter >= '0' && format_iter <= '9') {
        nonzero_padding = format_iter - '0';
        format_iter = *format++;
      }
      switch (format_iter) {
        case 'd':
        case 'u':
        case 'x':
          IntegerToArg(buffer, format_iter, *((int *)arg++));
          buffer_iter = buffer;
          goto string;
          break;
        case 's':
          buffer_iter = *arg++;
          if (!buffer_iter) buffer_iter = "(null)";
        string:
          for (buffer_end = buffer_iter; *buffer_end; buffer_end++) {
          }
          for (; buffer_end < buffer_iter + nonzero_padding; buffer_end++) {
            count += 1;
            VGAConsole_WriteChar(console, zero_padding ? '0' : ' ');
          }
          while (*buffer_iter) VGAConsole_WriteChar(console, *buffer_iter++);
          break;
        default:
          count += 1;
          VGAConsole_WriteChar(console, *((int *)arg++));
          break;
      }
    }
  }
  return count;
}

int VGAConsole_WriteN(VGAConsole *console, const char *data, uint32_t len) {
  for (uint32_t i = 0; i < len; ++i) VGAConsole_WriteChar(console, data[i]);
  return len;
}

int VGAConsole_Write(VGAConsole *console, const char *data) {
  return VGAConsole_WriteN(console, data, strlen(data));
}
