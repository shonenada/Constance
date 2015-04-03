rootfs: rootfs.img

rootfs.img:
	@rm -f bin/rootfs.img
	@bximage bin/rootfs.img -hd -mode=flat -size=1 -q
	@mkfs.minix bin/rootfs.img
	@mkdir /tmp/c_mnt_root
	@sudo mount -o loop -t minix bin/rootfs.img /tmp/c_mnt_root
	@mkdir -p ./bin/usr
	@gcc -o ./bin/usr/test.o $(CFLAGS) -c $(CINC)  -m32 ./usr/test.c -nostdinc -fno-builtin -fno-stack-protector
	@ld ./bin/usr/test.o -o ./bin/usr/test -T build/user.ld -m elf_i386
	@cp ./bin/usr/test root/bin/test
	@sudo cp -r ./root/* /tmp/c_mnt_root
	@sudo mknod /tmp/c_mnt_root/dev/tty0 c 1 0
	@sudo umount /tmp/c_mnt_root
	@rm -rf /tmp/c_mnt_root
