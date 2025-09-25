#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

typedef void exit_handler(void);
exit_handler h1, h2, h3;

int main(int argc, const char *argv[])
{
	atexit(h2);
	atexit(h1);
	atexit(h3);

	return 0;
}

void h1(void)
{
	printf("%s\n", "h1 called");
}

void h2(void)
{
	printf("%s\n", "h2 called");
	exit(0);
}

void h3(void)
{
	printf("%s\n", "h3 called");
}
