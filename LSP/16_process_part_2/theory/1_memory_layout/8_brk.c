/*
	As we can see from the  maps file, there was no [heap] region allocated.
	Let's do the same but with a program that calls malloc
*/

/* malloc uses brk internally. verify it by [ man malloc ] and [ strace malloc ] */

#include <stdio.h>
#include <stdlib.h>

/**
 * main - 1 call to malloc
 *
 * Return: EXIT_FAILURE if something failed. Otherwise EXIT_SUCCESS
 */
int main(void)
{
	void *p;

	p = malloc(1);
	printf("%p\n", p);
	getchar();
	return (EXIT_SUCCESS);
}
