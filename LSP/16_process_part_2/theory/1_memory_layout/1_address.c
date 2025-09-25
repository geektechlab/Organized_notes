/*
	We know that in C, local variables are located on the stack. 
	So if we print the address of a local variable, it should give us an idea on where we would find the stack in the virtual memory. 

*/

/* verify it by running,
-----
for ./1,
run [ ps -ef ]
find process id and then run [ cd /proc/<process id> ]
and then run [ cat maps ] and check heap and stack etc. addresses
-----
if it comes in teh stack range */

/* try running this multiple times and location will be different each time. This is done to avoid same stack adrress and hacking. It is called address space randomization and done in Linux */

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

	printf("Address of a: %p\n", (void *)&a);
	getchar();
	return (EXIT_SUCCESS);
}
