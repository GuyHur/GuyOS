#include <lib/system.h>

#define VGA_WIDTH 80
#define VGA_MEMORY (uint8_t*)0xB8000

using namespace guyos::common;
using namespace guyos::lib;

//kernel printf, prints in the framebuffer a string str, with an optional color, color.
void kprintf(const char* str, unsigned char color= (BACKGROUND_BLACK | FOREGROUND_WHITE))
{
    uint8_t* charPtr = (uint8_t*)str;
    uint16_t index = 0;
    while(*charPtr)
    {
        switch (*charPtr)
        {
        case 10:
            index += VGA_WIDTH;
            break;
        case 13:
            index -= index % VGA_WIDTH;
            break;
        default:
            *(VGA_MEMORY + index * 2) = *charPtr;
            *(VGA_MEMORY + index * 2 + 1) = color;
            index++;
        }
        charPtr++;
    }
};

void clear_screen(uint32_t clear_color= (BACKGROUND_BLACK | FOREGROUND_WHITE))
{
    uint32_t value = 0;
    value += clear_color << 8;
    value += clear_color << 16;
    value += clear_color << 24;
    for(uint32_t *i = (uint32_t*)VGA_MEMORY; i < (uint32_t*)VGA_MEMORY;i++)
    {
        *i = value;
    };
};

unsigned short cursor_pos; 

void set_cursor_pos(unsigned short position)
{
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
    
    cursor_pos = position;
};


