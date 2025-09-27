#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * main - uses strdup to create a new string, and prints the
 * address of the new duplcated string
 *
 * Return: EXIT_FAILURE if malloc failed. Otherwise EXIT_SUCCESS
 */

/* run [ man strdup ] for more details */

int main(void)
{
	char *s;

	s = strdup("Linux World");
	if (s == NULL)
	{
		fprintf(stderr, "Can't allocate mem with malloc\n");
		return (EXIT_FAILURE);
	}
	printf("%p\n", (void *)s);
	while(1);
	return (EXIT_SUCCESS);
}

//where do you think the duplicate string will be located? At a high or low memory address?
// low memory address because heap is in low memory addess