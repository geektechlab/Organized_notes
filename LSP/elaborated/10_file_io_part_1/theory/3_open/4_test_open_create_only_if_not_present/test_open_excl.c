#include <stdio.h>
#include <fcntl.h>

int main()
{
	/* check what following arguments mean */
	/* it will give error if file present else it will create file */
	/* generally both are used togather */
	int fd = open("hello.txt", O_WRONLY|O_CREAT|O_EXCL, 0666);
	if (fd < 0) {
		perror("open failed");
	} else {
		printf("Opening file successful\n");
		close(fd);
	}
	return 0;
}
