# yato

export SHELL := /bin/bash -o pipefail

# TODO(reach): aarch64 (ARM), riscv64 (RISCV)
ARCH := x86_64
QEMU := qemu-system-$(ARCH)

OS = $(shell uname -s)
BUILD_DIR := $(CURDIR)/build

CC := gcc
CFLAGS = -Wall -m32 -c -ffreestanding -O2 -lgcc -I.

LD := ld
LDFLAGS := -melf_i386

AS := as
ASFLAGS := --32

kernel_target := $(BUILD_DIR)/yato-$(ARCH).bin
img_target := $(BUILD_DIR)/yato-$(ARCH).iso

lib_string := $(BUILD_DIR)/lib/string/string.o

device_video_console := $(BUILD_DIR)/device/video/console/console.o

kernel: $(kernel_target)

$(kernel_target): binary_linker boot kernel_main $(lib_string) $(device_video_console)
	$(LD) $(LDFLAGS) -o $(kernel_target) $(lib_string) $(device_video_console) -T arch/$(ARCH)/linker.ld $(BUILD_DIR)/*.o

binary_linker: arch/$(ARCH)/linker.ld

boot: arch/$(ARCH)/boot.s
	@mkdir -p $(BUILD_DIR)
	$(AS) $(ASFLAGS) arch/$(ARCH)/boot.s -o $(BUILD_DIR)/boot.o

$(lib_string): lib/string/string.h lib/string/string.c
	@mkdir -p $(BUILD_DIR)/lib/string
	$(CC) $(CFLAGS) lib/string/string.c -o $(lib_string)

$(device_video_console): device/video/console/console.h device/video/console/console.c
	@mkdir -p $(BUILD_DIR)/device/video/console/
	$(CC) $(CFLAGS) device/video/console/console.c -o $(device_video_console)

kernel_main: arch/$(ARCH)/kernel_main.c
	$(CC) $(CFLAGS) arch/$(ARCH)/kernel_main.c -o $(BUILD_DIR)/kernel_main.o

img: $(img_target)

$(img_target): kernel
	@mkdir -p $(BUILD_DIR)/isofiles/boot/grub
	@cp $(kernel_target) $(BUILD_DIR)/isofiles/boot/kernel.bin
	@cp arch/$(ARCH)/grub.cfg $(BUILD_DIR)/isofiles/boot/grub
	@grub2-mkrescue -o $(img_target) $(BUILD_DIR)/isofiles
	@rm -r $(BUILD_DIR)/isofiles

qemu: img
	$(QEMU) -cdrom $(img_target) -vga std -no-reboot -d int,cpu_reset

clean:
	@test -d $(BUILD_DIR) && rm -rf $(BUILD_DIR) || true

# TODO: debugger target, after setting up serial port
# TODO: lint
# TODO: format (BSD style)
# TODO: sanitize
# TODO: testing
