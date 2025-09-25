#include <stdio.h>
#include <fcntl.h>

int main()
{
	/* it will truncate file size to 0 and make it empty ( deletes content ) */
	int fd = open("hello.txt", O_WRONLY|O_TRUNC);
	if (fd < 0) {
		perror("open failed");
	} else {
		printf("Opening file successful\n");
		close(fd);
	}
	return 0;
}
