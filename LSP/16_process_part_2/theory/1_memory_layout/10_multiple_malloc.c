/*
	What will happen if we call malloc several times?
*/

/* verify it by [ strace ./10 ], actually brk will be called initially once and later malloc will consume from large allocated memory only */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	void *p;

	write(1, "BEFORE MALLOC #0\n", 17);
	p = malloc(1024);
	write(1, "AFTER MALLOC #0\n", 16);
	printf("%p\n", p);

	write(1, "BEFORE MALLOC #1\n", 17);
	p = malloc(1024);
	write(1, "AFTER MALLOC #1\n", 16);
	printf("%p\n", p);

	write(1, "BEFORE MALLOC #2\n", 17);
	p = malloc(1024);
	write(1, "AFTER MALLOC #2\n", 16);
	printf("%p\n", p);

	write(1, "BEFORE MALLOC #3\n", 17);
	p = malloc(1024);
	write(1, "AFTER MALLOC #3\n", 16);
	printf("%p\n", p);

	getchar();
	return (EXIT_SUCCESS);
}
