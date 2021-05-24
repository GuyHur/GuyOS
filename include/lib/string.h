#ifndef __GUYOS__LIB__STRING_H
#define __GUYOS__LIB__STRING_H

namespace guyos
{
    namespace lib
    {
        void int_to_ascii(int n, char str[]);
        void reverse(char s[]);
        int strlen(char s[]);
        void backspace(char s[]);
        void append(char s[], char n);
        int strcmp(char s1[], char s2[]);
    }
}



#endif