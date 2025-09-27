#include <stdio.h>

/* encounter error by closing STDOUT */

int main()
{
	/* closing standard output */
	close(1);
	/* now printf will fail because standard output file descriptor is closed. we can verify with perror. also we can check with [ strace ./close_stdout.c ] */
	printf("Hello World");
	perror("printf");
	return 0;
}
