#include <stdio.h>

static int i = 100;

/* Declared as extern since defined in hello.c */
extern int global;

int function(char *input)
{
	printf("%s\n", input);
	return global;
}
