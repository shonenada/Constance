# output: bin/boot.bin

bin/boot.bin: src/boot/boot.asm
	@mkdir -p bin
	nasm src/boot/boot.asm -f bin -o bin/boot.bin

boot.img: bin/boot.bin
	@cat bin/boot.bin > boot.img

clean-boot:
	@rm -rf bin/boot.bin
	@rm boot.img
