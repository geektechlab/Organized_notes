#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

/* run [ man 3 open ] to check meaning of return value. 
here we are checking if return value is -1 then print open failed */

int main()
{
	int fd = open("hello.txt", O_RDWR);
	if (fd < 0) {
		printf("Open Failed\t error:%d\n", errno);
	} else {
		printf("Open Successful\n");
		close(fd);
	}
	return 0;
}

