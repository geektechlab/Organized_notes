#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* run ./1_hello and find return value by [ echo $? ] */

main(int argc, const char *argv[])
{
	printf("%s\n", "hello world");
}
