#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	sem_t *sem;
	int sem_value;

	sem = sem_open("linux", O_CREAT, 0666, 2);
	if (sem == SEM_FAILED) {
		perror("Semaphore open failed");
		exit(1);
	}

	if (sem_getvalue(sem, &sem_value) != 0) {
		perror("sem_getvalue");
	} else {
		printf("Semaphore value:%d\n", sem_value);
	}

	sem_unlink("linux");
	
	return 0;
}


