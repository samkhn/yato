# yato

export SHELL := /bin/bash -o pipefail

# TODO(reach): aarch64 (ARM)
ARCH := x86_64

QEMU := qemu-system-$(ARCH)

# You have a few options when displaying:
# -vga std  # to use system window manager/SDL
# -display curses  # libncurses
# -nographic  # command line only
# If you use curses or nographic, it's recommended you disable interrupt and cpu_reset logging (remove -d int,cpu_reset)
QEMUFLAGS := -no-reboot -no-shutdown # -d int,cpu_reset
QEMUSCREEN := -display curses

OS = $(shell uname -s)
BUILD_DIR := $(CURDIR)/build

# TODO: when switching architectures, we should set these to different toolchain
CC := gcc
CFLAGS = -Wall -m32 -ffreestanding -g -I. -nostdlib
LD := ld
LDFLAGS := -melf_i386
AS := as
ASFLAGS := -32

all: $(img)

# Libraries
lib_build_setup:
	@mkdir -p $(BUILD_DIR)/lib

lib_string := $(BUILD_DIR)/lib/string.o
$(lib_string): lib_build_setup lib/string.h lib/string.c
	$(CC) $(CFLAGS) -c lib/string.c -o $(lib_string)

lib_memory := $(BUILD_DIR)/lib/memory.o
$(lib_memory): lib_build_setup lib/memory.h lib/memory.c
	$(CC) $(CFLAGS) -c lib/memory.c -o $(lib_memory)

# TODO: blob/opaque ID generator
# TODO: key value store

# Interrupt tables
# Because descriptor tables here are specific to x86, we do not use $ARCH
interrupt_build_setup:
	@mkdir -p $(BUILD_DIR)/arch/x86_64/descriptor_tables

gdt_flush := $(BUILD_DIR)/arch/x86_64/descriptor_tables/gdt_flush.o
$(gdt_flush): interrupt_build_setup arch/x86_64/descriptor_tables/gdt_flush.S
	$(CC) $(CFLAGS) -c arch/x86_64/descriptor_tables/gdt_flush.S -o $(gdt_flush)

idt_flush := $(BUILD_DIR)/arch/x86_64/descriptor_tables/idt_flush.o
$(idt_flush): interrupt_build_setup arch/x86_64/descriptor_tables/idt_flush.S
	$(CC) $(CFLAGS) -c arch/x86_64/descriptor_tables/idt_flush.S -o $(idt_flush)

isr_common := $(BUILD_DIR)/arch/x86_64/descriptor_tables/isr_common.o
$(isr_common): interrupt_build_setup arch/x86_64/descriptor_tables/isr_common.s
	$(CC) $(CFLAGS) -c arch/x86_64/descriptor_tables/isr_common.s -o $(isr_common)

isr_stubs := $(BUILD_DIR)/arch/x86_64/descriptor_tables/isr_stubs.o
$(isr_stubs): interrupt_build_setup arch/x86_64/descriptor_tables/isr_stubs.s
	$(CC) $(CFLAGS) -c arch/x86_64/descriptor_tables/isr_stubs.s -o $(isr_stubs)

isr := $(BUILD_DIR)/arch/x86_64/descriptor_tables/isr.o
$(isr): interrupt_build_setup arch/x86_64/descriptor_tables/isr.h arch/x86_64/descriptor_tables/isr.c
	$(CC) $(CFLAGS) -c arch/x86_64/descriptor_tables/isr.c -o $(isr)

descriptor_tables := $(BUILD_DIR)/arch/x86_64/descriptor_tables/descriptor_tables.o
$(descriptor_tables): interrupt_build_setup arch/x86_64/descriptor_tables/descriptor_tables.h arch/x86_64/descriptor_tables/descriptor_tables.c
	$(CC) $(CFLAGS) -c arch/x86_64/descriptor_tables/descriptor_tables.c -o $(descriptor_tables)

# Boot utilities
boot_build_setup:
	@mkdir -p $(BUILD_DIR)/arch/$(ARCH)/boot/

boot_console := $(BUILD_DIR)/arch/$(ARCH)/boot/console.o
$(boot_console): boot_build_setup arch/$(ARCH)/boot/console.h arch/$(ARCH)/boot/console.c
	$(CC) $(CFLAGS) -c arch/$(ARCH)/boot/console.c -o $(boot_console)

boot_processed := $(BUILD_DIR)/arch/$(ARCH)/boot/boot.s
$(boot_processed): boot_build_setup arch/$(ARCH)/boot/boot.S
	$(CC) -E arch/$(ARCH)/boot/boot.S -I. > $(boot_processed)

boot := $(BUILD_DIR)/arch/$(ARCH)/boot/boot.o
$(boot): boot_build_setup $(boot_processed)
	$(AS) $(ASFLAGS) $(boot_processed) -o $(boot)

# Kernel
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
	@grub-mkrescue -o $(img) $(BUILD_DIR)/isofiles
	@rm -r $(BUILD_DIR)/isofiles

qemu: $(img)
	$(QEMU) $(QEMUFLAGS) -cdrom $(img) $(QEMUSCREEN)

clean:
	@test -d $(BUILD_DIR) && rm -rf $(BUILD_DIR) || true

format:
	@find . -name '*.c' -o -name '*.h' | xargs clang-format -i

# TODO: lint
# TODO: debugger target
# TODO: sanitize
# TODO: testing
