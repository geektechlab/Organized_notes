#include <stdio.h>
#include <sys/select.h>

/* here maximum file descriptor is not given, so it may block */

int main()
{
	int retval;
	fd_set readset, writeset, exceptset;

	FD_ZERO(&readset);
	FD_ZERO(&writeset);
	FD_ZERO(&exceptset);

	printf("fd of stdin:%d\n", fileno(stdin));
	FD_SET(fileno(stdin), &readset);
	printf("calling select\n");
	//if we don't give correct nfds
	retval = select(fileno(stdin), &readset, &writeset, &exceptset, NULL);
	printf("select returned:%d\n", retval);

	return 0;
}
