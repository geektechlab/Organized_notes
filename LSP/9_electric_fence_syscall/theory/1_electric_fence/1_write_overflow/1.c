#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* compile using [ gcc <file name.c> -o <output file> -lefence -g ] */
/* then run [ gdb ./1 ] */

int main()
{
	char *arr;
	int i;

	arr = (char *)malloc(sizeof(char)*5);
	strcpy(arr,"Linux is my name");
	return 0;
}
