
GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386
objects = loader.o kernel.o

%.o: %.cpp
		g++ $(GPPPARAMS) -o $@ -c $<

%.o : %.s
		as $(ASPARAMS) -o $@ $<

guykernel.bin : linker.ld $(objects)
		ld $(LDPARAMS) -T $< -o $@ $(objects)

install: guykernel.bin
		sudo cp $< /boot/guykernel.bin

guykernel.iso: guykernel.bin
		mkdir iso
		mkdir iso/boot
		mkdir iso/boot/grub
		cp $< iso/boot/
		echo 'set timeout=0' >> iso/boot/grub/grub.cfg
		echo 'set default=0' >> iso/boot/grub/grub.cfg
		echo 'menuentry "Guy OS" {' >> iso/boot/grub/grub.cfg
		echo 'multiboot /boot/guykernel.bin' >> iso/boot/grub/grub.cfg
		echo 'boot' >> iso/boot/grub/grub.cfg
		echo '}' >> iso/boot/grub/grub.cfg
		grub-mkrescue --output=$@ iso
		rm -rf iso
run: guykernel.iso
	(killall VBoxManage && sleep 1) || true
	VBoxManage startvm "Guy OS" &
