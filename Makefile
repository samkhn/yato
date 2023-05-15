# xkern

export SHELL := /bin/bash -o pipefail

# TODO(reach): aarch64 (ARM), riscv64 (RISCV)
ARCH := x86_64
QEMU := qemu-system-$(ARCH)

OS = $(shell uname -s)
BUILD_DIR := $(CURDIR)/build

CC := gcc
CFLAGS = -m64 -Wall -Werror -c -ffreestanding -O2 -lgcc

LD := ld
LDFLAGS := -melf_x86_64

AS := as
ASFLAGS := --64

kernel_target := $(BUILD_DIR)/xkern-$(ARCH).bin
img_target := $(BUILD_DIR)/xkern-$(ARCH).iso

kernel: $(kernel_target)

$(kernel_target): binary_linker boot kernel_main
	$(LD) $(LDFLAGS) -o $(kernel_target) -T arch/$(ARCH)/linker.ld $(BUILD_DIR)/*.o

binary_linker: arch/$(ARCH)/linker.ld

boot: arch/$(ARCH)/boot.s
	@mkdir -p $(BUILD_DIR)
	$(AS) $(ASFLAGS) arch/$(ARCH)/boot.s -o $(BUILD_DIR)/boot.o

kernel_main: arch/$(ARCH)/kernel_main.c
	$(CC) $(CFLAGS) arch/$(ARCH)/kernel_main.c -o $(BUILD_DIR)/kernel_main.o

img: $(img_target)

$(img_target): kernel
	@mkdir -p $(BUILD_DIR)/isofiles/boot/grub
	@cp $(kernel_target) $(BUILD_DIR)/isofiles/boot/kernel.bin
	@cp arch/$(ARCH)/grub.cfg $(BUILD_DIR)/isofiles/boot/grub
	@grub-mkrescue -o $(img_target) $(BUILD_DIR)/isofiles
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
