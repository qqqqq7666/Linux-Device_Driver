.PHONY kernel-download kernel-config kernel-add-systemcall kernel-build program-build
kernel-download:
	if [ -e kerenl ]; then \
	wget https://cdn.kerenl.org/pub/linux/kernel/v6.x/linux-6.2.5.tar.xz; tar xvf linux-6.2.5.tar.xz; rm linux-6.2.5.tar.xz; mv linux-6.2.5 kernel; \
	fi
kernel-config: kernel-download
	cp kernel_config kernel/.config
	cd kernel; ARCH=arm64 make olddefconfig
kernel-add-systemcall: kernel-config
	vi /home/kcjin/linux/include/uapi/asm-generic/unistd.h
kernel-build: kernel-config
	cd kernel; \
	ARCH=arm64 CROSS_COMPILE=`pwd`/home/kcjin/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu- make -j`nproc --all`
program-build: kernel-build kernel-add-systemcall
	/home/kcjin/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc -o $(syscall) $(cfile)
