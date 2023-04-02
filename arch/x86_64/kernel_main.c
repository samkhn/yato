// TODO: VGA stuff is for testing.
// Need to replace bc UEFI only supports Pixel Buffers.
// VGA in BIOS is deprecated

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void
kernel_main(void)
{
  const char space = ' ';
  const char *message = "xkern";
  const size_t message_length = 5;
  const size_t width = 80;
  const size_t height = 25;
  size_t terminal_row = 0;
  size_t terminal_column = 0;
  uint8_t terminal_color = 0 | 7 << 4;
  uint16_t* terminal_buffer = (uint16_t*) 0xB8000;

  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      const size_t i = y * width + x;
      terminal_buffer[i] = ((uint16_t)space) | (((uint16_t)terminal_color) << 8);
    }
  }
  for (size_t i = 0; i < message_length; ++i) {
    const size_t i = terminal_row * width + terminal_column;
    terminal_buffer[i] = ((uint16_t)message[i]) | (((uint16_t)terminal_color) << 8);
    if (++terminal_column == width) {
      terminal_column = 0;
      if (++terminal_row == height)
        terminal_row = 0;
    }
  }
}
