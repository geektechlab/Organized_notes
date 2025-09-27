#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/mman.h>

#define NAME	"/myshm"

int main() {

	int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
	if (fd < 0) {
		perror("shm_open()");
		return EXIT_FAILURE;
	}

	printf("open successful\n");
	close(fd);	
	shm_unlink(NAME);
	perror("shm_unlink");
}
