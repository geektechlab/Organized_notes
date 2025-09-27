#define _GNU_SOURCE  
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define STACK_SIZE	65536
int fd;

static int child_func(void *arg)
{
	close(fd);
	return 0;
}

int main(int argc, char *argv[])
{
	//Allocate stack for child task
	char *stack = malloc(STACK_SIZE);
	unsigned long flags = 0;
	char buf[256];
	int status;

	fd = open("file.txt", O_RDWR);
	if (fd == -1) {
		perror("Failed to open file\n");
		exit(1);
	}
	if (!stack) {
		perror("Failed to allocate memory\n");
		exit(1);
	}

	/* in this case CLONE_FILES will indicate file descriptor will be shared */
	if (argc == 2 && !strcmp(argv[1], "files")) 
		flags |= CLONE_FILES; 

	if (clone(child_func, stack + STACK_SIZE, flags | SIGCHLD, NULL) == -1) {
		perror("clone");
		exit(1);
	}

	if (wait(&status) == -1) {
		perror("Wait");
		exit(1);
	}	

	printf("Child exited with status:%d\n", status);
	status = read(fd, buf, 100);
	if (status < 0) {
		perror("Read Failed\n");
		exit(1);
	}
	printf("Parent read:%s\n", buf);
	close(fd);

	return 0;
}
