#ifndef __GUYOS__LIB__STRING_H
#define __GUYOS__LIB__STRING_H

#include <common/types.h>

namespace guyos
{
    namespace lib
    {
        void int_to_ascii(int n, char str[]);
        void reverse(char s[]);
        int strlen(char *str);
        void backspace(char s[]);
        void append(char *str, char c);
        int strncmp(const char *str, const char *str2, size_t n);
    }
}



#endif