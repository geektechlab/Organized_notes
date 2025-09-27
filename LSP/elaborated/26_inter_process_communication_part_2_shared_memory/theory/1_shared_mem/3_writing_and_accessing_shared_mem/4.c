#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/mman.h>

#define NAME	"/myshm"

#define MAPS_BUF_SIZE 4096

int main() {
	int *address;
	int fd = shm_open(NAME, O_RDWR, 0600);
	if (fd < 0) {
		perror("shm_open()");
		return EXIT_FAILURE;
	}

	/* prints memory map before and after mapping shared memory and writing to shared memory and reading back and then unmapping */

	printf("open successful\n");
	ftruncate(fd, 4096);
	perror("ftruncate");
	address = (int *)mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	perror("mmap");
 	printf("Mapped address: %p\n", address);
	for (int i = 0; i < 100; ++i) {
		printf("Address:%p\t Data:%d\n", address+i, address[i]);
	}	
	munmap(address, 4096);
	perror("munmap");
	close(fd);	
}
