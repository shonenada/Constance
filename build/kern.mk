# output: bin/kernel.bin

bin/kernel/scrn.o: src/kernel/scrn.c
	@gcc $(CFLAGS) $(CINC) -c -m32 -o bin/kernel/scrn.o src/kernel/scrn.c

bin/kernel/main.o: src/kernel/main.c
	@mkdir -p bin/kernel
	@gcc $(CFLAGS) $(CINC) -c -m32 -o bin/kernel/main.o src/kernel/main.c

bin/entry.o: src/kernel/entry.asm
	@mkdir -p bin
	@nasm src/kernel/entry.asm -f elf -o bin/entry.o

bin/kernel.elf: bin/entry.o bin/kernel/main.o bin/kernel/scrn.o
	@ld -o bin/kernel.elf -Ttext 0x1000 bin/entry.o bin/kernel/main.o bin/kernel/scrn.o -m elf_i386

bin/kernel.bin: bin/kernel.elf
	objcopy -R .pdr -R .comment -R .note -S -O binary $< $@

clean-kernel:
	@rm -rf bin/kernel
	@rm -rf bin/kernel.o
	@rm -rf bin/kernel.bin
