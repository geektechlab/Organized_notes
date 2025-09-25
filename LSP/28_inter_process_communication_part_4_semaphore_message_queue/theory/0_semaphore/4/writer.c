#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <limits.h>

#define NAME    "/myshm"

int *address;

void inc_count(int count)
{
	for (int i = 0; i < count; i++)
		(*address)++;
}

int main() {

	sem_t *sem;
	int fd = shm_open(NAME, O_RDWR|O_CREAT, 0600);
	ftruncate(fd, 4096);

	sem = sem_open("linux", O_CREAT, 0666, 1);


	address = (int *)mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        printf("%d:Mapped address: %p\n", getpid(), address);
	printf("%d: Trying to acquire semaphore\n", getpid());
	sem_wait(sem);
	printf("%d: Acquired Semaphore\n", getpid());
	inc_count(100000);
	munmap(address, 4096);
	sem_post(sem);
	printf("%d: Giving out semaphore\n", getpid());
	return 0;
}

