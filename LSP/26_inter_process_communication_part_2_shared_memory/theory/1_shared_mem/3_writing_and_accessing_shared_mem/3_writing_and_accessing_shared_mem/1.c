#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/mman.h>

#define NAME	"/myshm"

int main() {
	int *address;
	int fd = shm_open(NAME, O_RDWR, 0600);
	if (fd < 0) {
		perror("shm_open()");
		return EXIT_FAILURE;
	}
	printf("open successful\n");
	ftruncate(fd, 4096);
	perror("ftruncate");

	/* mapping shared memory adress to current adress space */ /* watch more about mmap at https://www.youtube.com/watch?v=8hVLcyBkSXY */
	address = (int *)mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	perror("mmap");
 	printf("Mapped address: %p\n", address);
	
	munmap(address, 4096);
	perror("munmap");
	close(fd);	
}
