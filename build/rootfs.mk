libsys_folder:
	@mkdir -p bin/libsys

libsys.s.o: libsys_folder usr/lib/entry.S
	@nasm -f elf -o bin/libsys/entry.o usr/lib/entry.S

libsys.c.o: libsys_folder usr/lib/libsys.c usr/lib/string.c usr/lib/printf.c
	@gcc -c $(CINC) $(CFLAGS) -m32 -nostdinc -fno-builtin -fno-stack-protector usr/lib/string.c -o bin/libsys/string.o
	@gcc -c $(CINC) $(CFLAGS) -m32 -nostdinc -fno-builtin -fno-stack-protector usr/lib/libsys.c -o bin/libsys/libsys.o
	@gcc -c $(CINC) $(CFLAGS) -m32 -nostdinc -fno-builtin -fno-stack-protector usr/lib/printf.c -o bin/libsys/printf.o

usr: libsys.s.o libsys.c.o
	@mkdir -p ./bin/usr
	@gcc -o ./bin/usr/test.o $(CFLAGS) -c $(CINC) -m32 ./usr/test.c -nostdinc -fno-builtin -fno-stack-protector
	@ld bin/libsys/entry.o bin/libsys/libsys.o bin/libsys/string.o bin/libsys/printf.o ./bin/usr/test.o -o ./bin/usr/test -T build/user.ld -m elf_i386

rootfs: usr rootfs.img

rootfs.img:
	@rm -f bin/rootfs.img
	@bximage bin/rootfs.img -hd -mode=flat -size=1 -q
	@mkfs.minix bin/rootfs.img
	@mkdir /tmp/c_mnt_root
	@sudo mount -o loop -t minix bin/rootfs.img /tmp/c_mnt_root
	@cp -r ./bin/usr root/bin
	@cp -r ./root/* /tmp/c_mnt_root
	@sudo mknod /tmp/c_mnt_root/dev/tty0 c 1 0
	@mkdir /tmp/c_mnt_root/usr
	@echo "hello from Constance" > /tmp/c_mnt_root/usr/hello.txt
	@sudo umount /tmp/c_mnt_root
	@rm -rf /tmp/c_mnt_root
