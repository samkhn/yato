#include "arch/x86_64/boot/console.h"
#include "arch/x86_64/boot/multiboot.h"

void kernel_main(unsigned long magic, unsigned long mbaddr) {
  VGAConsole console;
  VGAConsole_Initialize(&console, kVgaConsoleDefaultWidth,
                        kVgaConsoleDefaultHeight,
                        (VGAConsole_Pixel *)kVgaConsoleDefaultFBLocation,
                        VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    VGAConsole_Write(&console, "Invalid magic number");
  }
  MultibootInfo *multiboot_info;
  multiboot_info = (struct MultibootInfo *)mbaddr;
  VGAConsole_Write(&console, "\n\nHello yato\n\n");
  while (1) {
  }
}
