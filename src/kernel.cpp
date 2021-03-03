#include <common/types.h>
#include <gdt.h>
#include <hardware/interrupts.h>
#include <hardware/port.h>
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
    MouseDriver mouse(&interrupts);
    interrupts.Activate();

    while (1);
}