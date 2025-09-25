#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/mman.h>

#define NAME	"/myshm"

#define MAPS_BUF_SIZE 4096

static void print_maps()
{
	char line[MAPS_BUF_SIZE];
	int fd;

	/* /proc/self is for current process */ /* /proc/self/maps gives virtual address space for current process */
	fd = open("/proc/self/maps", O_RDONLY);
	perror("open maps");

	while(1)
	{
		memset(line, 0, sizeof(line));
		if (read(fd, line, MAPS_BUF_SIZE) == 0)
			break;
		printf("%s\n", line);
	}
	close(fd);
}

int main() {
	int *address;
	int fd = shm_open(NAME, O_RDWR, 0600);
	if (fd < 0) {
		perror("shm_open()");
		return EXIT_FAILURE;
	}

	/* prints memory map before and after mapping shared memory */

	printf("open successful\n");
	ftruncate(fd, 4096);
	perror("ftruncate");
	print_maps();
	getchar();
	address = (int *)mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	perror("mmap");
 	printf("Mapped address: %p\n", address);

	print_maps();
	getchar();
	munmap(address, 4096);
	print_maps();
	getchar();
	perror("munmap");
	close(fd);	
}
