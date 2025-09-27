#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int *addr;

	addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	switch (fork()) {
		case -1:
			perror("fork");
			exit(1);
			break;
		case 0:
			printf("Child started, value = %d\n", *addr);
			(*addr)++;
			munmap(addr, sizeof(int));
			exit(EXIT_SUCCESS);
		default:
			wait(NULL);
			printf("In parent, value = %d\n", *addr);
			munmap(addr, sizeof(int));
			exit(EXIT_SUCCESS);
	}
}
