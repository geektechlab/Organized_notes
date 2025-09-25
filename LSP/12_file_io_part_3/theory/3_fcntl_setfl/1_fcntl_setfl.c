
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void set_fl(int fd, int flags)
{
    int val;
    if((val = fcntl(fd, F_GETFL, 0)) < 0){
        fprintf(stderr, "fcntl F_GETFL error\n");
        exit(EXIT_FAILURE);
    }
    val |= flags;
    if (fcntl(fd, F_SETFL, val) < 0) {
        fprintf(stderr, "fcntl F_SETFL error\n");
        exit(EXIT_FAILURE);
    }
}

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
	/* opening file */
	int fd = open("hello.txt", O_WRONLY);
	perror("open");
	print_flags(fd);
	/* writing buffer content into file */
	write(fd, "hello", sizeof("hello"));
	perror("write");
	getchar();
	/* setting offset to start */
	lseek(fd, 0, SEEK_SET);
	/* changing file descriptor to append mode */
	set_fl(fd, O_APPEND);
	print_flags(fd);
	/* since set_fl has updated file descriptor, world will get appended to hello.txt */
	write(fd, "world", sizeof("world"));
	perror("write");
	close(fd);
	return 0;
}
