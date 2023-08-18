#include "arch/x86_64/boot/multiboot.h"
#include "arch/x86_64/boot/console.h"

void kernel_main(unsigned long magic, unsigned long mbaddr) {
  struct VGAConsole console;
  VGAConsole_Initialize(&console, kVgaConsoleDefaultWidth,
                        kVgaConsoleDefaultHeight,
                        (VGAConsole_Pixel *)kVgaConsoleDefaultFBLocation,
                        VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    VGAConsole_Write(&console, "Invalid magic number");
  }
  /* struct MultibootInfo *multiboot_info; */
  /* multiboot_info = (struct MultibootInfo *)mbaddr; */
  /* VGAConsole_Initiailze(&console, multiboot_info->framebuffer_width, */
  /*                       multiboot_info->framebuffer_height, */
  /*                       (VGAConsole_Pixel *)multiboot_info->framebuffer_addr,
   */
  /*                       VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK); */
  VGAConsole_Write(&console, "Hello yato");
  while (1) {
  }
}
