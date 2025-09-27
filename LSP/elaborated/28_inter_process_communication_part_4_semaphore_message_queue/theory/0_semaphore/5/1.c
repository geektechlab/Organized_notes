#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	sem_t sem;

	sem_init(&sem, 0, 1);
	perror("sem_init");
	sem_destroy(&sem);
	perror("sem_destroy");
	
	return 0;
}


