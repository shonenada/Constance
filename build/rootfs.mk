rootfs: rootfs.img

rootfs.img:
	@rm -f bin/rootfs.img
	@bximage bin/rootfs.img -hd -mode=flat -size=1 -q
	@mkfs.minix bin/rootfs.img
	@mkdir /tmp/c_mnt_root
	@sudo mount -o loop -t minix bin/rootfs.img /tmp/c_mnt_root
	@sudo cp -r ./root/* /tmp/c_mnt_root
	@sudo mknod /tmp/c_mnt_root/dev/tty0 c 1 0
	@sudo umount /tmp/c_mnt_root
	@rm -rf /tmp/c_mnt_root
