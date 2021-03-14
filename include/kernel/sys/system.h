#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <common/types.h>
/*
*kprintf - > kernel printf
*@params: str -> char pointer
*/
void printf(char *str);

uint32_t strlen(const char *str);
int strcmp(const char * a, const char * b);
extern char * strcpy(char * dest, const char * src);

extern void *memcpy(void *restrict dest, const void *restrict src, size_t count);
void * memset(void * b, int val, size_t count);

int max(int a, int b);

int min(int a, int b);






#endif