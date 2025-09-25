#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

/* STRERROR gives string equivalent of return value */

int main()
{
	int fd = open("hello.txt", O_RDWR);
	if (fd < 0) {
		printf("Open Failed\t error:%s\n", strerror(errno));
	} else {
		printf("Open Successful\n");
		close(fd);
	}
	return 0;
}

