#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd = open("hello.txt", O_RDONLY);
	char *address;

	if (fd < 0) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	address = mmap(NULL, 5, PROT_READ, MAP_SHARED, fd, 0);
	if (address == MAP_FAILED) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}
	
	/* here, we are not using read() / write() system calls */
	printf("%c ", address[0]);
	printf("%c ", address[1]);
	printf("%c ", address[2]);
	printf("%c ", address[3]);
	printf("%c ", address[4]);

	munmap(address, 5);
	close(fd);

	return 0;
}	
