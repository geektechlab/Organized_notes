#include <stdio.h>
#include <fcntl.h>

int main(void)
{
	int fd, fd2, fd3;

	if ((fd = fcntl(1, F_DUPFD, 0)) < 0)
		perror("fcntl-dup: fcntl error");
	printf("fd = %d\n", fd);

	if (write(fd, "tian\n", 5) != 5)
		printf("fcntl-dup: can't write to fd: %d", fd);

	if ((fd2 = fcntl(fd, F_DUPFD, 6)) < 0)
		perror("fcntl-dup: fcntl error");
	printf("fd2 = %d\n", fd2);

	if ((fd3 = fcntl(fd, F_DUPFD, 6)) < 0)
		perror("fcntl-dup: fchtl error");
	printf("fd3 = %d\n", fd3);

	close(fd);
	close(fd2);
	close(fd3);
	exit(0);

}
