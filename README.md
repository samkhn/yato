# yato

yato := yet another toy OS

## Architectures

x86 32 bit at the moment. Future work will focus on transitioning to x86 64 bit, then added ARMv8 support, then RISCV support.

## Build

Please have the following programs in your path:

* GNU Make
* GNU Compiler Collection and GNU BinUtils (we use gcc, as, ld). Must be able to compile 32 bit ELF targets
* xorriso, grub-mkrescue
* qemu-system-x86
* (if you want to send a PR) clang-format

## Run
```bash
$ make qemu
```

### Possible issues

* image not building on Ubuntu?

Might need to run `sudo apt install grub-pc-bin`.
