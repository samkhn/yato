#include "arch/x86_64/descriptor_tables/descriptor_tables.h"

#define GDT_ENTRY_COUNT 5
#define IDT_ENTRY_COUNT 256

static gdt_entry_t GDT_ENTRIES[GDT_ENTRY_COUNT];
static gdt_pointer_t GDT_POINTER;
static idt_entry_t IDT_ENTRIES[IDT_ENTRY_COUNT];
static idt_pointer_t IDT_POINTER;

static void set_gdt_gate(int32_t num, uint32_t base, uint32_t limit,
			 uint8_t access, uint8_t granularity)
{
	GDT_ENTRIES[num].base_low = (base & 0xFFFF);
	GDT_ENTRIES[num].base_middle = (base >> 16) & 0xFF;
	GDT_ENTRIES[num].base_high = (base >> 24) & 0xFF;
	GDT_ENTRIES[num].limit_low = (limit & 0xFFFF);
	GDT_ENTRIES[num].granularity = (limit >> 16) & 0x0F;
	GDT_ENTRIES[num].granularity |= granularity & 0xF0;
	GDT_ENTRIES[num].access = access;
}

static void set_idt_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flag)
{
	IDT_ENTRIES[num].base_low = (base & 0xFFFF);
	IDT_ENTRIES[num].base_high = (base >> 16 & 0xFFFF);
	IDT_ENTRIES[num].kernel_segment_selector = sel;
	IDT_ENTRIES[num].always_zero = 0;
	// TODO: when we get to user-mode, we uncomment the or below
	// Sets interrupt gate privilege to level 3.
	IDT_ENTRIES[num].flags = flag /*| 0x60*/;
}

void init_dt()
{
	init_gdt();
	init_idt();
}

void init_gdt()
{
	GDT_POINTER.limit = (sizeof(gdt_entry_t) * GDT_ENTRY_COUNT) - 1;
	GDT_POINTER.base = (uint32_t)&GDT_ENTRIES;
	set_gdt_gate(0, 0, 0, 0, 0); // null entry
	set_gdt_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // kernel code segment
	set_gdt_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // kernel data segment
	set_gdt_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // userspace code segment
	set_gdt_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // userspace data segment
	gdt_flush((uint32_t)&GDT_POINTER);
}

void init_idt()
{
	IDT_POINTER.limit = (uint16_t)sizeof(idt_entry_t) * IDT_ENTRY_COUNT - 1;
	IDT_POINTER.base = (uint32_t)&IDT_ENTRIES;
	//memset(&IDT_ENTRIES, 0, sizeof(idt_entry_t) * 256);
	set_idt_gate(0, (uint32_t)isr0, 0x08, 0x8E);
	set_idt_gate(1, (uint32_t)isr1, 0x08, 0x8E);
	set_idt_gate(2, (uint32_t)isr2, 0x08, 0x8E);
	set_idt_gate(3, (uint32_t)isr3, 0x08, 0x8E);
	set_idt_gate(4, (uint32_t)isr4, 0x08, 0x8E);
	set_idt_gate(5, (uint32_t)isr5, 0x08, 0x8E);
	set_idt_gate(6, (uint32_t)isr6, 0x08, 0x8E);
	set_idt_gate(7, (uint32_t)isr7, 0x08, 0x8E);
	set_idt_gate(8, (uint32_t)isr8, 0x08, 0x8E);
	set_idt_gate(9, (uint32_t)isr9, 0x08, 0x8E);
	set_idt_gate(10, (uint32_t)isr10, 0x08, 0x8E);
	set_idt_gate(11, (uint32_t)isr11, 0x08, 0x8E);
	set_idt_gate(12, (uint32_t)isr12, 0x08, 0x8E);
	set_idt_gate(13, (uint32_t)isr13, 0x08, 0x8E);
	set_idt_gate(14, (uint32_t)isr14, 0x08, 0x8E);
	set_idt_gate(15, (uint32_t)isr15, 0x08, 0x8E);
	set_idt_gate(16, (uint32_t)isr16, 0x08, 0x8E);
	set_idt_gate(17, (uint32_t)isr17, 0x08, 0x8E);
	set_idt_gate(18, (uint32_t)isr18, 0x08, 0x8E);
	set_idt_gate(19, (uint32_t)isr19, 0x08, 0x8E);
	set_idt_gate(20, (uint32_t)isr20, 0x08, 0x8E);
	set_idt_gate(21, (uint32_t)isr21, 0x08, 0x8E);
	set_idt_gate(22, (uint32_t)isr22, 0x08, 0x8E);
	set_idt_gate(23, (uint32_t)isr23, 0x08, 0x8E);
	set_idt_gate(24, (uint32_t)isr24, 0x08, 0x8E);
	set_idt_gate(25, (uint32_t)isr25, 0x08, 0x8E);
	set_idt_gate(26, (uint32_t)isr26, 0x08, 0x8E);
	set_idt_gate(27, (uint32_t)isr27, 0x08, 0x8E);
	set_idt_gate(28, (uint32_t)isr28, 0x08, 0x8E);
	set_idt_gate(29, (uint32_t)isr29, 0x08, 0x8E);
	set_idt_gate(30, (uint32_t)isr30, 0x08, 0x8E);
	set_idt_gate(31, (uint32_t)isr31, 0x08, 0x8E);
	idt_flush((uint32_t)&IDT_POINTER);
}
