// TODO: VGA stuff is for testing.
// Need to replace bc UEFI only supports Pixel Buffers.
// VGA in BIOS is deprecated

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void kernel_main(void)
{
  const char message[5] = "xkern";
  const size_t message_length = 5;
  uint8_t terminal_color = 7 | 0 << 4;
  uint16_t* terminal_buffer = (uint16_t*) 0xB8000;
  for (size_t i = 0; i < message_length; ++i) {
    terminal_buffer[i] = ((uint16_t)message[i]) | (((uint16_t)terminal_color) << 8);
  }
}

