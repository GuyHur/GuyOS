#ifndef __GUYOS__LIB__SYSTEM_H
#define __GUYOS__LIB__SYSTEM_H

#include <common/types.h>

#define FOREGROUND_BLACK 0x00
#define FOREGROUND_BLUE 0x01
#define FOREGROUND_GREEN 0x02
#define FOREGROUND_CYAN 0x03
#define FOREGROUND_RED 0x04
#define FOREGROUND_MAGENTA 0x05
#define FOREGROUND_BROWN 0x06
#define FOREGROUND_LIGHTGRAY 0x07
#define FOREGROUND_DARKGRAY 0x08
#define FOREGROUND_LIGHTBLUE 0x09
#define FOREGROUND_LIGHTGREEN 0x0A
#define FOREGROUND_LIGHTCYAN 0x0B
#define FOREGROUND_LIGHTRED 0x0C
#define FOREGROUND_LIGHTMAGENTA 0x0D
#define FOREGROUND_YELLOW 0x0E
#define FOREGROUND_WHITE 0x0F

#define BACKGROUND_BLACK 0x00
#define BACKGROUND_BLUE 0x10
#define BACKGROUND_GREEN 0x20
#define BACKGROUND_CYAN 0x30
#define BACKGROUND_RED 0x40
#define BACKGROUND_MAGENTA 0x50
#define BACKGROUND_BROWN 0x60
#define BACKGROUND_LIGHTGRAY 0x70
#define BACKGROUND_BLINKINGBLACK 0x80
#define BACKGROUND_BLINKINGBLUE 0x90
#define BACKGROUND_BLINKINGGREEN 0xA0
#define BACKGROUND_BLINKINGCYAN 0xB0
#define BACKGROUND_BLINKINGRED 0xC0
#define BACKGROUND_BLINKINGMAGENTA 0xD0
#define BACKGROUND_BLINKINGYELLOW 0xE0
#define BACKGROUND_BLINKINGWHITE 0xF0


using namespace guyos::common;

void printf(char *str);

void help_command()
{
    printf("\tGuyOS\n");
    printf("$ Features:\n");
    printf("$ Network Stack\n");
    printf("$ Syscalls\n");
    printf("$ Driver System\n");
    printf("$ Multitasking\n");
    printf("$ And more!\n");
}

void init_shell();

void clear_screen()
{
    static uint16_t *VideoMemory = (uint16_t*)0xb8000;
    static uint8_t x = 0, y = 0;
    for(y = 0; y < 25; y++)
    {
        for(x = 0; x < 80; x++)
        {
            VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
        }
    }
}

void kprintf(char* c)
{
    static uint8_t x=0, y=0;
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    for(int i = 0; c[i] != '\0'; ++i)
    {


        switch (c[i])
        {
        case '\n':
            x =0;
            y++;
            break;
        case '\t':
            x += 4;
            
        default:
            VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | c[i];
        }
        if(x >= 80)
        {
            x = 0;
            y++;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';//clears the entire screen
            x = 0;
            y = 0;
        }

    }
};

#endif