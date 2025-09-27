/*
	The heap is used when you malloc space for your variables. Let's add a line to use malloc and see where the memory address returned by malloc is located 
*/

/* verify it by running,
-----
for ./1,
run [ ps -ef ]
find process id and then run [ cd /proc/<process id> ]
and then run [ cat maps ] and check heap and stack etc. addresses
-----
if it comes in teh heap range */


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
	getchar();
	return (EXIT_SUCCESS);
}
