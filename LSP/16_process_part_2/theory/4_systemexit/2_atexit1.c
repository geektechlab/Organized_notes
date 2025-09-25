#include <unistd.h>		/* sysconf() */
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

static void my_exit1(void);
static void my_exit2(void);
static void my_exit3(void);

int main(void)
{

	if (atexit(my_exit2) != 0)
		printf("can't register my_exit2: %s\n", strerror(errno));
	if (atexit(my_exit1) != 0)
		printf("can't register my_exit1: %s\n", strerror(errno));
	if (atexit(my_exit1) != 0)
		printf("can't register my_exit1: %s\n", strerror(errno));
	if (atexit(my_exit3) != 0)
		printf("can't register my_exit3: %s\n", strerror(errno));

	return 0;
}

static void my_exit1(void)
{
	printf("first exit handler\n");
}

static void my_exit2(void)
{
	printf("second exit handler\n");
}

static void my_exit3(void)
{
	printf("third exit handler\n");
}
