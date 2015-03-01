CINC := -Isrc/inc
CFLAGS := -Wall -nostdinc -fno-builtin -fno-stack-protector \
		 -finline-functions -finline-small-functions -findirect-inlining \
		 -finline-functions-called-once

include build/boot.mk
include build/kern.mk

boot: boot.img

kernel: kernel.img

bochs: kernel.img
	@mkdir -p log
	bochs -f .bochsrc -q

debug: kernel.img
	@mkdir -p log
	bochs-dbg -f .bochsrc -q

boot.img: bin/boot.bin
	@cat bin/boot.bin > boot.img

kernel.img: bin/boot.bin bin/kernel.bin
	@cat bin/boot.bin bin/kernel.bin > kernel.img

clean: clean-boot clean-kernel
	@rm -rf kernel.img

