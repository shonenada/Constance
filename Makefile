include build/boot.mk

boot: boot.img

bochs: boot
	@mkdir -p log
	bochs -f .bochsrc -q

clean: clean-boot

