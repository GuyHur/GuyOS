#include <common/types.h>
#include <gdt.h>
#include <hardware/interrupts.h>
#include <hardware/port.h>
#include <hardware/pci.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>

void printf(char *str)
{
    static uint16_t *VideoMemory = (uint16_t *)0xb8000;

    static uint8_t x = 0, y = 0;

    for (int i = 0; str[i] != '\0'; i++)
    // loop until null terminator found
    {
        switch (str[i])
        {
        case '\n':
            y++;
            x = 0;
            break;

        default:
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
            x++;
        }

        if (x >= 80)
        {
            y++;
            x = 0;
        }
        if (y >= 25)
        {
            for (y = 0; y < 25; y++)
            {
                for (x = 0; x < 80; x++)
                {
                    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' '; //Clearing the screen setting everything to blank
                }
            }

            x = 0;
            y = 0;
        }
    }
}

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        char* foo = " ";
        foo[0] = c;
        printf(foo);
    }
};

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:
    
    MouseToConsole()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);        
    }
    
    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }
    
};




typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for (constructor *i = &start_ctors; i != &end_ctors; i++)
    {
        (*i)();
    }
}

extern "C" void kernelMain(void *multiboot_structure, uint32_t magicnumber)
{
    printf("Hello world! --- github.com/GuyHur");

    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);

    printf("Initializing hardware, Stage 1 \n");
    //TODO: write keyboardhandler
    DriverManager drvManager;

    PrintfKeyboardEventHandler keyboardhandler;
    KeyboardDriver keyboard(&interrupts, &keyboardhandler);
    drvManager.addDriver(&keyboard);

    MouseToConsole mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler);

    PCI pcicontroller;
    pcicontroller.SelectDrivers(&drvManager);

    printf("Initializing hardware, stage 2\n");

    drvManager.ActivateAll();

    printf("Activating interrupts, stage 3 \n");
    interrupts.Activate();

    while (1);
}
