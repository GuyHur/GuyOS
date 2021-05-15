GCCPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
          obj/gdt.o \
		  obj/memory.o \
          obj/hardware/port.o \
          obj/hardware/interruptstubs.o \
          obj/hardware/interrupts.o \
		  obj/syscalls.o \
		  obj/multitasking.o \
		  obj/drivers/amd_am79c973.o \
		  obj/hardware/pci.o \
          obj/drivers/keyboard.o \
		  obj/net/etherframe.o \
		  obj/net/arp.o \
		  obj/net/ipv4.o \
          obj/drivers/mouse.o \
		  obj/drivers/driver.o \
		  obj/drivers/ata.o \
		  obj/drivers/vga.o \
          obj/kernel.o


run: kernel.iso
	(killall VirtualBox && sleep 1) || true
	VBoxManage startvm 'GuyOS' &

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -c -o $@ $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

kernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

kernel.iso: kernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp kernel.bin iso/boot/kernel.bin
	echo 'set timeout=0' 				> iso/boot/grub/grub.cfg
	echo 'set default=0' 				>> iso/boot/grub/grub.cfg
	echo '' 							>> iso/boot/grub/grub.cfg
	echo 'menuentry "Guy OS" {' 		>> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' 						>> iso/boot/grub/grub.cfg
	echo '}' 							>> iso/boot/grub/grub.cfg
	grub-mkrescue --output=kernel.iso iso
	rm -rf iso

install: kernel.bin
	sudo cp $< /boot/kernel.bin

.PHONY: clean
clean:
	rm -rf obj kernel.bin kernel.iso