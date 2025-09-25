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

	if ((fd = dup(STDOUT_FILENO)) < 0) {
        printf("dup: dup error: %s\n", strerror(errno));
        exit(1);
    }

	/* here STDOUT_FILENO is not closed because dup is used */
	printf("STDOUT_FILENO = %d, fd = %d\n", STDOUT_FILENO, fd);
	dup2fd = 5;

	if (dup2(fd, dup2fd) < 0) {
        printf("dup: dup2 error: %s\n", strerror(errno));
        exit(1);
    }
    printf("fd = %d, dup2fd = %d\n", fd, dup2fd);

	if (write(fd, "tianxia\n", 8) != 8) {
        printf("dup: can't write to %d: %s\n", fd, strerror(errno));
        exit(1);
    }

	if (write(5, "tianxia\n", 8) != 8) {
        printf("dup: can't write to %d: %s\n", fd, strerror(errno));
        exit(1);
    }
	return 0;
}
