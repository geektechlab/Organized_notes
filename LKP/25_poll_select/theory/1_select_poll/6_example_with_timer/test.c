#include <stdio.h>
#include <sys/select.h>
#include <stdlib.h>

int main()
{
	int retval;
	fd_set readset, writeset, exceptset;
	char buffer[10];
	struct timeval tv;

	FD_ZERO(&readset);
	FD_ZERO(&writeset);
	FD_ZERO(&exceptset);

	printf("fd of stdin:%d\n", fileno(stdin));
	FD_SET(fileno(stdin), &readset);

	//wait up to 10 seconds
	tv.tv_sec = 10;
	tv.tv_usec = 0;

	printf("calling select\n");
	retval = select(fileno(stdin) + 1, &readset, &writeset, &exceptset, &tv);
	printf("select returned:%d\n", retval);

	if (retval == -1)
	{
		perror("select() failed");
		exit(EXIT_FAILURE);
	}
	else if (retval == 0)
	{
		printf("No key pressed in 10 seconds exiting\n");
		exit(EXIT_SUCCESS);
	}

	if (FD_ISSET(fileno(stdin), &readset)) 
	{
		printf("calling read\n");
		retval = read(fileno(stdin), buffer, sizeof(buffer));
		buffer[retval] = '\0';
		printf("buffer:%s\n", buffer);
	}

	return 0;
}
