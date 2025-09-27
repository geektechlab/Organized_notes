#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, const char *argv[])
{
	int val;
	if (argc != 2) {
		fprintf(stderr, "usage: a.out fd\n");
		exit(EXIT_FAILURE);
	}
	if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0) {
		fprintf(stderr, "fcntl error for fd %d\n", atoi(argv[1]));
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
    return 0;
}
