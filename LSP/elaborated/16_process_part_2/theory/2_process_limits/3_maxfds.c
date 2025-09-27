#include <stdio.h>		/* for fprintf(), stderr, BUFSIZ */
#include <unistd.h>		/* for ssize_t */

/* getdtablesize() gives file descriptor table size */

int
main(int argc, char **argv)
{
	printf("max fds: %d\n", getdtablesize());
	exit(0);
}
