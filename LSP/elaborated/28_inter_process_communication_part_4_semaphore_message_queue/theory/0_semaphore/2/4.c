#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <stdlib.h>

void print_sem_value(sem_t *sem)
{
	int sem_value;

	if (sem_getvalue(sem, &sem_value) != 0) {
		perror("sem_getvalue");
	} else {
		printf("Semaphore value:%d\n", sem_value);
	}

}

int main(int argc, char *argv[])
{
	sem_t *sem;

	sem = sem_open("linux", O_CREAT, 0666, 2);
	if (sem == SEM_FAILED) {
		perror("Semaphore open failed");
		exit(1);
	}

	print_sem_value(sem);
	sem_wait(sem);
	print_sem_value(sem);
	
	return 0;
}


