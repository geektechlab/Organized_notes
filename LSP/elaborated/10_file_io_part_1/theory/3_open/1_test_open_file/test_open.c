#include <stdio.h>
/* if we do [ man 2 open ], we will get to know fcntl is required */
#include <fcntl.h>

int main()
{
	//Fails when there is no file exist
	//Create a file by running touch hello.txt
	//Change the flag to O_RDONLY, O_RDWR and compile
	/* try keeping and removing hello.txt in directory and see what happens */
	int fd = open("hello.txt", O_WRONLY);
	if (fd < 0) {
		/* if file descriptor is non-positive, then print following line with error message */
		perror("open failed");
	} else {
		printf("Opening file successful\n");
		close(fd);
	}
	return 0;
}
