
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