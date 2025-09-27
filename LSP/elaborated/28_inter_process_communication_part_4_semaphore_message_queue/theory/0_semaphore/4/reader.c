#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

#define NAME    "/myshm"

int *address;

void read_count()
{
	printf("Count is %d\n",(*address));
}

int main() {

	int fd = shm_open(NAME, O_RDWR|O_CREAT, 0600);
	perror("open");

	ftruncate(fd, 4096);
	perror("ftruncate");

	address = (int *)mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        perror("mmap");
        printf("Mapped address: %p\n", address);

	read_count();
	munmap(address, 4096);
        perror("munmap");
	close(fd);
	return 0;
}

