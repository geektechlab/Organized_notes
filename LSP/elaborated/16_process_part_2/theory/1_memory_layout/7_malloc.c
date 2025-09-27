/*
	malloc is the common function used to dynamically allocate memory. This memory is allocated on the "heap". 
	 Note: malloc is not a system call.

	void *malloc(size_t size);

	The malloc() function allocates size bytes and returns a pointer to the allocated memory.

	Let's look at memory regions of a process that does not call malloc
*/

/* nOnwadyas,Linux adds 0 heap even if program doesn't want */

#include <stdlib.h>
#include <stdio.h>
int main(void)
{
	getchar();
	return (EXIT_SUCCESS);
}
