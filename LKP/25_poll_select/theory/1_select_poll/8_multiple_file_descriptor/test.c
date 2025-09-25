#include <stdio.h>
#include <sys/select.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main()
{
	int retval;
	fd_set readset, writeset, exceptset;
	char buffer[10];
	struct termios ts;

	int fd = open("./myfifo", O_RDWR);
	perror("open");

	FD_ZERO(&readset);
	FD_ZERO(&writeset);
	FD_ZERO(&exceptset);

	printf("fd of stdin:%d\n", fileno(stdin));
	FD_SET(fileno(stdin), &readset);
	FD_SET(fd, &readset);

	printf("calling select\n");
	retval = select(fd + 1, &readset, &writeset, &exceptset, NULL);
	printf("select returned:%d\n", retval);

	if (retval == -1)
	{
		perror("select() failed");
		exit(EXIT_FAILURE);
	}
	else if (retval == 0)
	{
		printf("No data available\n");
		exit(EXIT_SUCCESS);
	}

	if (FD_ISSET(fileno(stdin), &readset)) 
	{
		printf("Data available in stdin\n");
		printf("calling read\n");
		retval = read(fileno(stdin), buffer, sizeof(buffer));
		buffer[retval] = '\0';
		printf("buffer:%s\n", buffer);
	}

	if (FD_ISSET(fd, &readset)) 
	{
		printf("Data available in fifo\n");
		printf("calling read\n");
		retval = read(fd, buffer, sizeof(buffer));
		printf("read returned:%d\n", retval);
		buffer[retval] = '\0';
		printf("buffer:%s\n", buffer);
	}

	close(fd);
	return 0;
}
