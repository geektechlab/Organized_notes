#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    int fd, assignvalue, dup2fd, fd2;
    int n;
    off_t offset;
    char buf[64];

	if ((fd = open("test", O_RDONLY)) < 0) {
        perror("open test error");
        exit(1);
    }

	if ((fd2 = dup(fd)) < 0) {
        perror("dup error");
        exit(1);
    }
    printf("fd = %d, fd2 = %d\n", fd, fd2);

	if ((n = read(fd, buf, 12)) < 0) {
        perror("read fd error");
        exit(1);
    }
    printf("read return: %d\n", n);

	if ((offset = lseek(fd, 0, SEEK_CUR)) < 0) {
        perror("lseek error");
        exit(1);
    }

	printf("lseek fd=%d, the current offset is: %lu\n", fd, offset);

	 if ((offset = lseek(fd2, 0, SEEK_CUR)) < 0) {
        perror("lseek error");
        exit(1);
    }
    printf("lseek fd2=%d, the current offset is: %lu\n", fd2, offset);
    close(fd);
    close(fd2);

	exit(0);
}
