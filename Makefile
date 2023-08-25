# yato

export SHELL := /bin/bash -o pipefail

# TODO(reach): aarch64 (ARM)
ARCH := x86_64
QEMU := qemu-system-$(ARCH)

OS = $(shell uname -s)
BUILD_DIR := $(CURDIR)/build

# TODO: when switching architectures,
# 	we should set these to different toolchain
CC := gcc
CFLAGS = -Wall -m32 -ffreestanding -g -I. -nostdlib
LD := ld
LDFLAGS := -melf_i386
AS := as
ASFLAGS := -32

all: $(img)

lib_string := $(BUILD_DIR)/lib/string.o
$(lib_string): lib/string.h lib/string.c
	@mkdir -p $(BUILD_DIR)/lib
	$(CC) $(CFLAGS) -c lib/string.c -o $(lib_string)

lib_memory := $(BUILD_DIR)/lib/memory.o
$(lib_memory): lib/memory.h lib/memory.c
	@mkdir -p $(BUILD_DIR)/lib
	$(CC) $(CFLAGS) -c lib/memory.c -o $(lib_memory)

# TODO: blob/opaque ID generator
# TODO: key value store

boot_console := $(BUILD_DIR)/arch/$(ARCH)/boot/console.o
$(boot_console): arch/$(ARCH)/boot/console.h arch/$(ARCH)/boot/console.c
	@mkdir -p $(BUILD_DIR)/arch/$(ARCH)/boot/
	$(CC) $(CFLAGS) -c arch/$(ARCH)/boot/console.c -o $(boot_console)

# Because descriptor tables here are specific to x86, we do not use $ARCH
gdt_flush := $(BUILD_DIR)/arch/x86_64/descriptor_tables/gdt_flush.o
$(gdt_flush): arch/x86_64/descriptor_tables/gdt_flush.S
	@mkdir -p $(BUILD_DIR)/arch/x86_64/descriptor_tables
	$(CC) $(CFLAGS) -c $^ -o $@

idt_flush := $(BUILD_DIR)/arch/x86_64/descriptor_tables/idt_flush.o
$(idt_flush): arch/x86_64/descriptor_tables/idt_flush.S
	@mkdir -p $(BUILD_DIR)/arch/x86_64/descriptor_tables
	$(CC) $(CFLAGS) -c $^ -o $@

isr_common := $(BUILD_DIR)/arch/x86_64/descriptor_tables/isr_common.o
$(isr_common): arch/x86_64/descriptor_tables/isr_common.s
	@mkdir -p $(BUILD_DIR)/arch/x86_64/descriptor_tables
	$(CC) $(CFLAGS) -c $^ -o $@

isr_stubs := $(BUILD_DIR)/arch/x86_64/descriptor_tables/isr_stubs.o
$(isr_stubs): arch/x86_64/descriptor_tables/isr_stubs.s
	@mkdir -p $(BUILD_DIR)/arch/x86_64/descriptor_tables
	$(CC) $(CFLAGS) -c $^ -o $@

isr := $(BUILD_DIR)/arch/x86_64/descriptor_tables/isr.o
$(isr): arch/x86_64/descriptor_tables/isr.h arch/x86_64/descriptor_tables/isr.c
	@mkdir -p $(BUILD_DIR)/arch/x86_64/descriptor_tables
	$(CC) $(CFLAGS) -c arch/x86_64/descriptor_tables/isr.c -o $(isr)

descriptor_tables := $(BUILD_DIR)/arch/x86_64/descriptor_tables/descriptor_tables.o
$(descriptor_tables): arch/x86_64/descriptor_tables/descriptor_tables.h arch/x86_64/descriptor_tables/descriptor_tables.c
	@mkdir -p $(BUILD_DIR)/arch/x86_64/descriptor_tables
	$(CC) $(CFLAGS) -c arch/x86_64/descriptor_tables/descriptor_tables.c -o $(descriptor_tables)

boot_processed := $(BUILD_DIR)/arch/$(ARCH)/boot/boot.s
$(boot_processed): arch/$(ARCH)/boot/boot.S
	@mkdir -p $(BUILD_DIR)/arch/$(ARCH)/boot
	$(CC) -E arch/$(ARCH)/boot/boot.S -I. > $(boot_processed)

boot := $(BUILD_DIR)/arch/$(ARCH)/boot/boot.o
$(boot): $(boot_processed)
	@mkdir -p $(BUILD_DIR)/arch/$(ARCH)/boot
	$(AS) $(ASFLAGS) $(boot_processed) -o $(boot)

kernel_main := $(BUILD_DIR)/kernel_main.o
$(kernel_main): arch/$(ARCH)/kernel_main.c
	$(CC) $(CFLAGS)	-c arch/$(ARCH)/kernel_main.c -o $(BUILD_DIR)/kernel_main.o

kernel := $(BUILD_DIR)/yato-$(ARCH).bin
$(kernel): arch/$(ARCH)/linker.ld $(boot) $(kernel_main) $(isr) $(isr_common) $(isr_stubs) $(descriptor_tables) $(idt_flush) $(gdt_flush) $(boot_console) $(lib_memory) $(lib_string)
	$(LD) $(LDFLAGS) -o $@ $(boot) $(kernel_main) $(isr) $(isr_common) $(isr_stubs) $(descriptor_tables) $(idt_flush) $(gdt_flush) $(boot_console) $(lib_memory) $(lib_string) -T arch/$(ARCH)/linker.ld

# Disk formats
img := $(BUILD_DIR)/yato-$(ARCH).iso
$(img): $(kernel) arch/$(ARCH)/grub.cfg
	@mkdir -p $(BUILD_DIR)/isofiles/boot/grub
	@cp $(kernel) $(BUILD_DIR)/isofiles/boot/kernel.bin
	@cp arch/$(ARCH)/grub.cfg $(BUILD_DIR)/isofiles/boot/grub
	@grub2-mkrescue -o $(img) $(BUILD_DIR)/isofiles
	@rm -r $(BUILD_DIR)/isofiles

qemu: $(img)
	$(QEMU) -cdrom $(img) -nographic -display curses \
		-no-reboot -d int,cpu_reset -m 512M

qemu-graphical: $(img)
	$(QEMU) -cdrom $(img) -vga std \
		-no-reboot -d int,cpu_reset -m 512M

clean:
	@test -d $(BUILD_DIR) && rm -rf $(BUILD_DIR) || true

format:
	@find . -name '*.c' -o -name '*.h' | xargs clang-format -i

# TODO: debugger target, after setting up serial port
# TODO: lint
# TODO: sanitize
# TODO: testing
