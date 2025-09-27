#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

static void parent(void)
{
	sem_t * sem_id;
	sem_id=sem_open("mysem", O_CREAT, 0600, 0);
	if(sem_id == SEM_FAILED) {
		perror("parent sem_open");
		return;
	}
	printf("waiting for child\n");
	if(sem_wait(sem_id) < 0) {
		perror("sem_wait");
	}
}

static void child(void)
{
	sem_t * sem_id;
	sem_id=sem_open("mysem", O_CREAT, 0600, 0);
	if(sem_id == SEM_FAILED) {
		perror("child sem_open");
		return;
	}
	printf("Posting for parent\n");
	if(sem_post(sem_id) < 0) {
		perror("sem_post");
	}
}

int main(int argc, char *argv[])
{
	pid_t pid;
	pid=fork();
	if(pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if(!pid) {
		child();    
	} else {
		int status;
		parent();
		wait(&status);
	}
	return 0;
}
