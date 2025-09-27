#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

/* Write your own dup2() function that performs the same service as the
 * dup2() function without calling the fcntl()
 * function. Be sure to handle errors correctly.
 */

/* following is implementation for the same */

int dup2_l(int oldfd, int newfd)
{
	int fd[256] = {0};
	int i;

	if (newfd >= 256 || newfd < 0)
		return -1;

	if (lseek(oldfd, 0, SEEK_CUR) < 0 && errno == EBADF) {
		printf("file descriptor: %d is not valid, %m\n", oldfd);
		return -1;
	}

	if (oldfd == newfd)
		return newfd;

	close(newfd);	

	for (i = 0; i < 256; ++i) {
		if ((fd[i] = dup(oldfd)) < 0) {
			printf("dup: %s\n", strerror(errno));
			while (--i > 0)
				close(fd[i]);
			return -1;
		}

		if (fd[i] == newfd)
			break;
	}

	if (i >= 256)
		return -1;

	while (--i >= 0)
		close(fd[i]);

	return newfd;
}


int main(void)
{
	int fd, n;
	char buf[16];


	if ((fd = open("new", O_RDWR | O_CREAT, 0644)) < 0)
		printf("use_dup2: can't open or create the file (new)");

	if (dup2_l(fd, 4) < 0)
		printf("use_dup2: can't dup2_l the file descriptor: %d", fd);

		if (write(4, "tian", 4) != 4)
		printf("use_dup2: can't write 'tian' to file (new)");

	if (write(fd, "xia", 3) != 3)
		printf("use_dup2: can't write 'xia' to file (new)");

	if (lseek(fd, 0, SEEK_SET) < 0)
		printf("use_dup2: cant' lseek the file (new)");

	if ((n = read(fd, buf, 16)) < 0)
		printf("use_dup2_l: can't read from the file (new)");
	buf[n] = '\0';
	printf("buf: %s\n", buf);

	if (close(fd) < 0)
		printf("use_dup2: can't close the file descriptor: %d", fd);
	if (close(4) < 0)
		printf("use_dup2: can't close the file descriptor: 4");

	exit(0);
}
