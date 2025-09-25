#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

/* check snip about perror.
perror is unbuffered I/O and directly prints even if we don't add \n in that at end of string.
printf is buffered I/O and we have to add \n at the end to get string printed.
\n flushes the buffer or we can call fflush(<standard output macro>) to flush I/O.*/

int main()
{
	int fd = open("hello.txt", O_RDWR);
	if (fd < 0) {
		perror("Open Failed");
	} else {
		printf("Open Successful\n");
		close(fd);
	}
	return 0;
}

