#include "arch/x86_64/boot/console.h"
#include "arch/x86_64/boot/multiboot.h"
#include "arch/x86_64/descriptor_tables/descriptor_tables.h"

void kernel_main(uint32_t magic, uint32_t mbaddr)
{
	bprintf("\n\nHello yato\n\n");
	// test_bprintf(/*halt=*/1);

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		bprint("Invalid magic number\n");
	} else {
		bprintf("Magic number found: 0x%x\n", magic);
	}

	multiboot_info_t *mbi = (multiboot_info_t *)mbaddr;
	if (mbi->flags & MULTIBOOT_INFO_MEM_MAP) {
		bprintf("mem upper = 0x%x, mem lower = 0x%d\n\n",
			mbi->mem_upper, mbi->mem_lower);
		bprintf("mmap_addr = 0x%x, mmap_length = %d\n", mbi->mmap_addr,
			mbi->mmap_length);
	}
	for (int i = 0; i < mbi->mmap_length;
	     i += sizeof(multiboot_memory_map_t)) {
		multiboot_memory_map_t *mmmt =
			(multiboot_memory_map_t *)(mbi->mmap_addr + i);
		bprintf("Start Addr: 0x%x | Length: 0x%x | Size: %d | Type: %d\n",
			(uint32_t)mmmt->addr, (uint32_t)mmmt->len, mmmt->size,
			mmmt->type);
	}

	init_dt();
	bprintf("\nGDT and IDT is set\n");
	asm volatile("int $0x3");

	while (1) {
	}
}
