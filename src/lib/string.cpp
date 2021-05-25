#include <lib/string.h>

using namespace guyos;
using namespace guyos::common;
using namespace guyos::lib;


int strlen(const char *str)
{
    size_t count = 0;

    while(*str++)
    {
        count++;
    }
    return count;
};

int strncmp(const char *str, const char *str2, size_t n)
{
    uint8_t u1, u2;

    while(n-- > 0)
    {
        u1 = (uint8_t)*str++;
        u2 = (uint8_t)*str2++;
        if(u1 != u2)
            return u1 - u2;
        if(u1 == '\0')
            return 0;
    }
    return 0;
}