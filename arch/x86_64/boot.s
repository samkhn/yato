  // Multiboot header
  .set  ALIGN,    1<<0
  .set  MEMINFO,  1<<1
  .set  FLAGS,    ALIGN | MEMINFO
  .set  MAGIC,    0x1BADB002
  .set  CHECKSUM, -(MAGIC + FLAGS)

  .section multiboot
  .align 4
  .long MAGIC
  .long FLAGS
  .long CHECKSUM

  // Stack creation
  .section bss
  .align 16
stack_bottom:
  .skip 16384  # 16 KiB
stack_top:

  // Bootloader will jump here
  .section .text
  .global _start
  .type _start, @function
_start:
  mov $stack_top, %esp
  // TODO: setup GDT
  // TODO: setup paging
  call kernel_main
  cli

hltLoop:
  hlt
  jmp hltLoop

.size _start, . - _start
  
