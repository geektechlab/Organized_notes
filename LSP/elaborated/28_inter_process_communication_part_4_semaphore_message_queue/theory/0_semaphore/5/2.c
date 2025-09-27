#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	sem_t sem;
	int value;

	sem_init(&sem, 0, 5);
	perror("sem_init");
	sem_getvalue(&sem, &value);
	perror("sem_getvalue");
	printf("semaphore value:%d\n", value);
	sem_destroy(&sem);
	perror("sem_destroy");
	
	return 0;
}


