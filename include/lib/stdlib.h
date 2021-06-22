#ifndef STDLIB_H
#define STDLIB_H 1

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__))
void abort();

char* itoa(int value, char* str, int base);

#ifdef __cplusplus
}
#endif

#endif