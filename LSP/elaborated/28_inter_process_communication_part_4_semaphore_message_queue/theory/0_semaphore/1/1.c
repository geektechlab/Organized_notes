#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	sem_t *sem;

	sem = sem_open("/linux", O_CREAT, 0666, 1);
	if (sem == SEM_FAILED) {
		perror("Semaphore open failed");
		exit(1);
	}
	getchar();
	sem_unlink("/linux");
	
	return 0;
}


