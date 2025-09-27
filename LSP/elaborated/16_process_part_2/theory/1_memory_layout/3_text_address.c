/*
	Your program is also in the virtual memory. If we print the address of the main function, we should have an idea of where the program is located compared to the stack and the heap
*/

/* verify it by running,
-----
for ./1,
run [ ps -ef ]
find process id and then run [ cd /proc/<process id> ]
and then run [ cat maps ] and check heap and stack etc. addresses
-----
if it comes in the text range */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * main - print locations of various elements
 *
 * Return: EXIT_FAILURE if something failed. Otherwise EXIT_SUCCESS
 */
int main(void)
{
	int a;
	void *p;

	printf("Address of a: %p\n", (void *)&a);
	p = malloc(98);
	if (p == NULL)
	{
		fprintf(stderr, "Can't malloc\n");
		return (EXIT_FAILURE);
	}
	printf("Allocated space in the heap: %p\n", p);
	printf("Address of function main: %p\n", (void *)main);
	getchar();
	return (EXIT_SUCCESS);
}
