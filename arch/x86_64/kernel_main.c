#include "arch/x86_64/boot/console.h"
#include "arch/x86_64/boot/multiboot.h"

void kernel_main(uint32_t magic, uint32_t mbaddr)
{
	vga_console_t console;
	vga_console_init(&console, VGA_CONSOLE_DEFAULT_WIDTH,
			 VGA_CONSOLE_DEFAULT_HEIGHT,
			 (vga_console_pixel_t *)VGA_CONSOLE_DEFAULT_FB_ADDR,
			 VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	vga_console_printf(&console, "\n\nHello yato\n\n");
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
		vga_console_print(&console, "Invalid magic number");
	multiboot_info_t *mbi = (multiboot_info_t *)mbaddr;
	if (mbi->flags & MULTIBOOT_INFO_MEM_MAP) {
		vga_console_printf(&console,
				   "mmap_addr = 0x%x, mmap_length = 0x%x\n",
				   (uint32_t)mbi->mmap_addr,
				   (uint32_t)mbi->mmap_length);
	}
	while (1) {
	}
}
