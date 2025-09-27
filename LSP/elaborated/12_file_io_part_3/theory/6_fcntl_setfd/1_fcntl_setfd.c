#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void set_fd(int fd, int flags)
{
    int val;
    if((val = fcntl(fd, F_GETFD, 0)) < 0){
        fprintf(stderr, "fcntl F_GETFD error\n");
        exit(EXIT_FAILURE);
    }
    val |= flags;
    if (fcntl(fd, F_SETFD, val) < 0) {
        fprintf(stderr, "fcntl F_SETFD error\n");
        exit(EXIT_FAILURE);
    }
}

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

	/* update file descriptor flag */
	set_fd(fd, FD_CLOEXEC);
	getchar();
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
