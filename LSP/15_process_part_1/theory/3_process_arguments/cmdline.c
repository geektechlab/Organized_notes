#include <stdio.h>

/* When a program is executed, the process that does the exec() can pass
 * command-line arguments to the new program.
 */
//int main(int argc, char *gc])
int main(int count, char *gc[])
{
	int i;

	for (i = 0; i < count; ++i)	/* echo all command-line args */
		printf("gc[%d]: %s\n", i, gc[i]);

	printf("Again.....................\n");

	/* We are guaranteed by by both ISO C and POSIX.1 that gccount] is
	 * a null pointer. This lets us alternatively code the
	 * argument-processing loop as
	 */
	for (i = 0; gc[i] != NULL; ++i)
		printf("gc[%d]: %s\n", i, gc[i]);

	return 0;
}
