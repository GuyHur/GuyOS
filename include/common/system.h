#ifndef __GUYOS__COMMON__SYSTEM_H
#define __GUYOS__COMMON__SYSTEM_H

namespace guyos
{
    namespace common
    {
        //Memory
        extern unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);
        extern unsigned char *memset(unsigned char *dest, unsigned char val, int count);
        extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
        //String

        extern int strlen(const char *str);


        //Screen
        extern void cls();// clears screen
        extern void putch(unsigned char c); // puts char on the screen
        extern void puts(unsigned char *str);// puts string on the screen
        extern void settextcolor(unsigned char forecolor, unsigned char backcolor);//sets the text color
        extern void init_video();// initializes the video
        
    }
}
#endif