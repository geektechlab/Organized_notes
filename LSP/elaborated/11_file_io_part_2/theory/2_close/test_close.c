#include <stdio.h>
#include <fcntl.h>

int main()
{
	//Fails when there is no file exist
	//Create a file by running touch hello.txt
	//Change the flag to O_RDONLY, O_RDWR and compile
	int fd = open("hello.txt", O_WRONLY);
	if (fd < 0) {
		perror("open failed");
	} else {
		printf("Opening file successful\n");
		/* this will fail */
		close(fd);
		perror("close");
		/* this will pass */
		close(10);
		perror("close");
	}
	return 0;
}
