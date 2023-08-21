#include "arch/x86_64/boot/console.h"
#include "arch/x86_64/boot/multiboot.h"

void kernel_main(uint32_t magic, uint32_t mbaddr) {
  VGAConsole console;
  VGAConsole_Initialize(&console, kVgaConsoleDefaultWidth,
                        kVgaConsoleDefaultHeight,
                        (VGAConsole_Pixel *)kVgaConsoleDefaultFBLocation,
                        VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  VGAConsole_Write(&console, "\n\nHello yato\n\n");
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    VGAConsole_Write(&console, "Invalid magic number");
  MultibootInfo *multiboot_info = (MultibootInfo *)mbaddr;
  if (multiboot_info->flags & MULTIBOOT_INFO_MEM_MAP) {
    VGAConsole_WriteF(&console, "mmap_addr = 0x%x, mmap_length = 0x%x\n",
                      (uint32_t)multiboot_info->mmap_addr,
                      (uint32_t)multiboot_info->mmap_length);
  }
  while (1) {
  }
}
