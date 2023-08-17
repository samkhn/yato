# yato

export SHELL := /bin/bash -o pipefail

# TODO(reach): aarch64 (ARM)
ARCH := x86_64
QEMU := qemu-system-$(ARCH)

OS = $(shell uname -s)
BUILD_DIR := $(CURDIR)/build

# TODO: when switching architectures, we should set these to different toolchain
CC := gcc
CFLAGS = -Wall -m32 -c -ffreestanding -O2 -I.
LD := ld
LDFLAGS := -melf_i386
AS := as
ASFLAGS := --32

all: format img

# Libraries
lib_string := $(BUILD_DIR)/lib/string/string.o
libraries: $(lib_string)
$(lib_string): lib/string/string.h lib/string/string.c
	@mkdir -p $(BUILD_DIR)/lib/string
	$(CC) $(CFLAGS) lib/string/string.c -o $(lib_string)
# TODO: blob/opaque ID generator
# TODO: key value store

# Device drivers
device_video_console := $(BUILD_DIR)/device/video/console/console.o
drivers: $(device_video_console)
$(device_video_console): device/video/console/console.h device/video/console/console.c
	@mkdir -p $(BUILD_DIR)/device/video/console/
	$(CC) $(CFLAGS) device/video/console/console.c -o $(device_video_console)

# Main kernel build artifacts
boot_processed := $(BUILD_DIR)/boot.s
boot_target := $(BUILD_DIR)/boot.o
boot: $(boot_target)
$(boot_target): $(boot_processed)
	@mkdir -p $(BUILD_DIR)
	$(AS) $(ASFLAGS) $(boot_processed) -o $(boot_target)
$(boot_processed): arch/$(ARCH)/boot.S
	@mkdir -p $(BUILD_DIR)
	$(CC) -E arch/$(ARCH)/boot.S -I. > $(boot_processed)

kernel_main_target := $(BUILD_DIR)/kernel_main.o
kernel_main: $(kernel_main_target)
$(kernel_main_target): arch/$(ARCH)/kernel_main.c
	$(CC) $(CFLAGS) arch/$(ARCH)/kernel_main.c -o $(BUILD_DIR)/kernel_main.o

kernel_target := $(BUILD_DIR)/yato-$(ARCH).bin
kernel: $(kernel_target)
# TODO: replace hard coded library and device drivers with list variables?
$(kernel_target): arch/$(ARCH)/linker.ld boot kernel_main libraries drivers
	$(LD) $(LDFLAGS) -o $(kernel_target) $(BUILD_DIR)/*.o $(lib_string) $(device_video_console) -T arch/$(ARCH)/linker.ld

# Disk formats
img_target := $(BUILD_DIR)/yato-$(ARCH).iso
img: $(img_target)
$(img_target): kernel
	@mkdir -p $(BUILD_DIR)/isofiles/boot/grub
	@cp $(kernel_target) $(BUILD_DIR)/isofiles/boot/kernel.bin
	@cp arch/$(ARCH)/grub.cfg $(BUILD_DIR)/isofiles/boot/grub
	@grub2-mkrescue -o $(img_target) $(BUILD_DIR)/isofiles
	@rm -r $(BUILD_DIR)/isofiles

qemu: img
	$(QEMU) -nographic -cdrom $(img_target) -display curses -no-reboot -d int,cpu_reset

qemu-graphical: img
	$(QEMU) -cdrom $(img_target) -no-reboot -d int,cpu_reset

clean:
	@test -d $(BUILD_DIR) && rm -rf $(BUILD_DIR) || true

format:
	@find . -name '*.c' -o -name '*.h' | xargs clang-format -i

# TODO: debugger target, after setting up serial port
# TODO: lint
# TODO: sanitize
# TODO: testing
