#include <lib/stdio.h>


#include <tty.h>


int putchar(int ic)
{

	char c = (char) ic;
	term_write(&c, sizeof(c));

	// TODO: You need to implement a write system call.
	return ic;
}
