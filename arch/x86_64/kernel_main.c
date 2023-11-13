#include "arch/x86_64/boot/console.h"
#include "arch/x86_64/boot/multiboot.h"
#include "arch/x86_64/descriptor_tables/descriptor_tables.h"

void kernel_main(uint32_t magic, uint32_t mbaddr)
{
	bprintf("\n\nHello yato\n\n");

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
		bprint("Invalid magic number\n");
	else
		bprintf("Magic number found: 0x%x\n", magic);

	multiboot_info_t *mbi = (multiboot_info_t *)mbaddr;
	if (mbi->flags & MULTIBOOT_INFO_MEM_MAP) {
		bprintf("mmap_addr = 0x%x, mmap_length = %d\n",
			(uint32_t)mbi->mmap_addr, (uint32_t)mbi->mmap_length);
	}
	for (multiboot_memory_map_t *mmap =
		     (multiboot_memory_map_t *)(uint32_t)mbi->mmap_addr;
	     mmap < (multiboot_memory_map_t *)mbi->mmap_addr + mbi->mmap_length;
	     mmap = (multiboot_memory_map_t *)((uint32_t)mmap + mmap->size +
					       sizeof(mmap->size))) {
		bprintf(" mmap at base: 0x%x , length: %u, type: %u ",
			mmap->addr, (uint32_t)mmap->len, (uint32_t)mmap->type);
	}

	init_dt();
	bprintf("\nGDT and IDT is set\n");
	asm volatile("int $0x3");

	while (1) {
	}
}
