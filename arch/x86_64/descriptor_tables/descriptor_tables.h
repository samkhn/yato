#ifndef YATO_ARCH_X86_64_BOOT_DESCRIPTOR_TABLES_H
#define YATO_ARCH_X86_64_BOOT_DESCRIPTOR_TABLES_H

#include <stdint.h>

#include "lib/memory.h"

struct gdt_entry {
	uint16_t limit_low; // Lower 16 bits of the limit
	uint16_t base_low; // Lower 16 bits of the base
	uint8_t base_middle; // followed by middle 8 bits
	uint8_t access; // ending with high 8 bits
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed));
typedef struct gdt_entry gdt_entry_t;

struct gdt_pointer {
	uint16_t limit; // Upper 16 bits of all selector limits
	uint32_t base; // Address of 1st GDT
} __attribute__((packed));
typedef struct gdt_pointer gdt_pointer_t;

struct idt_entry {
	// base_low and high represent the lower 16 and upper 16 bits of the address
	// to jump to when an interrupt fires.
	uint16_t base_low;
	uint16_t kernel_segment_selector;
	uint8_t always_zero;
	uint8_t flags;
	uint16_t base_high;
} __attribute__((packed));
typedef struct idt_entry idt_entry_t;

struct idt_pointer {
	uint16_t limit; // Upper 16 bits of all selector limits
	uint32_t base; // Address of 1st IDT
} __attribute__((packed));
typedef struct idt_pointer idt_pointer_t;

extern void gdt_flush(uint32_t gdt_pointer);
extern void idt_flush(uint32_t idt_pointer);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

void init_gdt();
void init_idt();
void init_dt();

#endif // YATO_ARCH_X86_64_BOOT_DESCRIPTOR_TABLES_H
