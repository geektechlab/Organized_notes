#include <stdio.h>
#include <fcntl.h>

/* copy content from one file to other using buffer. Uses open, read and write system calls */

#define BUFFSIZE 4096
int main(void)
{
	int n;
	char buf[BUFFSIZE];
	int fd1, fd2;

	/* see arguments used in open for different files */
	fd1 = open("hello.txt", O_RDONLY);
	perror("open hello.txt");
	fd2 = open("newhello.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	perror("open newhello.txt");
	while ((n = read(fd1, buf, BUFFSIZE)) > 0)
		if (write(fd2, buf, n) != n)
			perror("write error");
	if (n < 0)
		perror("read error");
	close(fd1);
	close(fd2);
	exit(0);
}
