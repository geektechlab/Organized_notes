#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void print_flags(int fd)
{
    int val;
	if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
		fprintf(stderr, "fcntl error for fd %d\n", fd);
		exit(EXIT_FAILURE);
	}
	switch(val & O_ACCMODE)
	{
		case O_RDONLY:
			printf("read only\n");
			break;
		case O_WRONLY:
			printf("write only\n");
			break;
		case O_RDWR:
			printf("read write\n");
			break;
		default:
			fprintf(stderr, "unknown access mode\n");
			exit(EXIT_FAILURE);
			break;
	}
	if (val & O_APPEND) {
		printf(", append");
	}
    if (val & O_NONBLOCK) {
        printf(", nonblocking");
    }
    if (val & O_SYNC) {
        printf(", synchronous writes");
    }
#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
    if (val & O_FSYNC) {
        printf(", synchronous writes");
    }
#endif
    putchar('\n');
}

int main()
{
	int filedes[2];
	char buffer[] = "hello pipe";
	char readbuffer[15] = {};

	if (pipe(filedes) == 0) {
		printf("Pipe successful\n");
		printf("read from %d, write to %d\n", filedes[0], filedes[1]);
		write(filedes[1], buffer, sizeof(buffer));
		read(filedes[0], readbuffer, sizeof(readbuffer));
		printf("read:%s\n", readbuffer);
		close(filedes[1]);
		close(filedes[0]);
	} else {
		perror("pipe failed");
	}
	return 0;
}
