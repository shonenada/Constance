# output: bin/kernel.bin

HEADER_FILES = $(wildcard src/**/*.h)
C_FILES = $(wildcard src/**/*.c)
ASM_FILES = src/kernel/entry.asm
OBJ_FILES = $(addprefix bin/, $(notdir $(C_FILES:.c=.o) $(ASM_FILES:.asm=.o)))

bin/%.o: src/**/%.c $(HEADER_FILES)
	gcc -o $@ $(CFLAGS) $(CINC) -c -m32 $<

bin/entry.o: src/kernel/entry.asm
	@mkdir -p bin
	nasm src/kernel/entry.asm -f elf -o bin/entry.o

bin/kernel.elf: $(OBJ_FILES) build/kern.ld
	ld $(OBJ_FILES) -o $@ -T build/kern.ld -m elf_i386

bin/kernel.bin: bin/kernel.elf
	objcopy -R .pdr -R .comment -R .note -S -O binary $< $@

clean-kernel:
	@rm -rf bin/*.o
	@rm -rf bin/**/*.o
	@rm -rf bin/kernel.bin
