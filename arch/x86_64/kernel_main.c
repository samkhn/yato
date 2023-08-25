#include "arch/x86_64/boot/console.h"
#include "arch/x86_64/boot/multiboot.h"
#include "arch/x86_64/descriptor_tables/descriptor_tables.h"

void kernel_main(uint32_t magic, uint32_t mbaddr)
{
	bprintf("\n\nHello yato\n\n");

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
		bprint("Invalid magic number");
	multiboot_info_t *mbi = (multiboot_info_t *)mbaddr;
	if (mbi->flags & MULTIBOOT_INFO_MEM_MAP) {
		bprintf("mmap_addr = 0x%x, mmap_length = 0x%x\n",
			(uint32_t)mbi->mmap_addr, (uint32_t)mbi->mmap_length);
	}

	init_dt();
	asm volatile("int $0x3");

	while (1) {
	}
}
