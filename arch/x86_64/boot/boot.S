#define ASM_FILE 1
#include "arch/x86_64/boot/multiboot.h"

#define AOUT_KLUDGE 0  // Because we are producing an ELF binary
#define MULTIBOOT_HEADER_FLAGS MULTIBOOT_PAGE_ALIGNMENT | MULTIBOOT_MEMORY_INFO | MULTIBOOT_VIDEO_MODE

	.set FLAGS,	MULTIBOOT_HEADER_FLAGS
	.set MAGIC,	MULTIBOOT_HEADER_MAGIC
	.set CHECKSUM,	-(MAGIC + FLAGS)

.section .multiboot
	.align	4
	// Header
	.long 	MAGIC
	.long 	FLAGS
	.long 	CHECKSUM
	// AOut (because the kernel is in ELF form, this is unused)
	.long 	0
	.long 	0
	.long 	0
	.long 	0
	.long 	0
	// Video info
	// TODO: make this configurable at build time
	.long 	1
	.long 	1024
	.long 	768
	.long 	16

	// bootstrap stack creation
.section .bss
	.align 16
stack_bottom:
	.skip	16384
stack_top:

	// Bootloader will jump here
.section .text
	.global	_start
	.type	_start, @function
_start:
	mov	$stack_top,	%esp
	// TODO: setup GDT
	// TODO: setup paging
	pushl	$0
	popf

	pushl	%ebx
	pushl	%eax

	call	kernel_main
	cli

	// TODO: assert that .size isn't too large within the linker script
.size _start, . - _start