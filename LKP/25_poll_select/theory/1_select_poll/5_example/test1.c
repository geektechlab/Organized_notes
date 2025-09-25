#include <stdio.h>
#include <sys/select.h>

int main()
{
	int retval;
	fd_set readset, writeset, exceptset;
	char buffer[10];

	FD_ZERO(&readset);
	FD_ZERO(&writeset);
	FD_ZERO(&exceptset);

	printf("fd of stdin:%d\n", fileno(stdin));
	FD_SET(fileno(stdin), &readset);
	printf("calling select\n");
	retval = select(fileno(stdin) + 1, &readset, &writeset, &exceptset, NULL);
	printf("select returned:%d\n", retval);

	if (FD_ISSET(fileno(stdin), &readset)) 
	{
		printf("calling read\n");
		retval = read(fileno(stdin), buffer, sizeof(buffer));
		buffer[retval] = '\0';
		printf("buffer:%s\n", buffer);
	}

	return 0;
}
