xkern

Just another toy OS.

Targets x86. Will work on ARM and RISCV in the future.

Requires
- make
- gcc, ld, as
- xorriso, grub-mkrescue
- qemu-system-x86

Run:
$ make qemu

Common issues

* image not building? Might need to run
$ sudo apt-get install grub-pc-bin
