# output: bin/boot.bin

# bin/boot.bin: src/boot/boot.asm
	# @mkdir -p bin
	# nasm src/boot/boot.asm -f bin -o bin/boot.bin

bin/boot.o: src/boot/boot.asm
	@mkdir -p bin
	@nasm src/boot/boot.asm -f elf -o bin/boot.o

bin/boot.bin: bin/boot.o
	@ld bin/boot.o -o bin/boot.bin -T build/boot.ld -m elf_i386

clean-boot:
	@rm -rf bin/boot.bin
