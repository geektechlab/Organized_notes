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
	sem_t sem;
	sem_init(&sem, 0, 3);

	print_sem_value(&sem);
	sem_wait(&sem);
	print_sem_value(&sem);
	sem_post(&sem);
	print_sem_value(&sem);
	sem_destroy(&sem);
	
	return 0;
}


