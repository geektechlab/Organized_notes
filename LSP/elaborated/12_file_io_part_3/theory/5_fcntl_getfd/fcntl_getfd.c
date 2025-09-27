#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, const char *argv[])
{
	int val;
	int fd; 
	fd = open("hello.txt", O_RDONLY);
	perror("open");
	if ((val = fcntl(fd, F_GETFD, 0)) < 0) {
		fprintf(stderr, "fcntl error for fd %d\n", atoi(argv[1]));
		exit(EXIT_FAILURE);
	}
	if (val & FD_CLOEXEC) {
		printf("file descriptor close over an execve()\n");
	} else {
		printf("file descriptor open over an execve()\n");
	}
	close(fd);
	return 0;
}
