#include <stdio.h>
#include <stdlib.h>

/* check with strace */

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

	address = (int *)mmap(0, 4096, PROT_WRITE , MAP_SHARED, fd, 0);
	perror("mmap");
 	printf("Mapped address: %p\n", address);
	address[4096] = 11;
	munmap(address, 4096);
	perror("munmap");
	close(fd);	
}
