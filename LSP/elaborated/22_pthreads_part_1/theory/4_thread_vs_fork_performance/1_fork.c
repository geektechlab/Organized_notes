#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUMFORKS 50000

void child_process(void)
{
	int i = 0;
	i++;
}

int main(void)
{
	int pid, j, status;

	for (j = 0; j < NUMFORKS; j++) {
		switch (pid = fork()) {
		case -1:
			printf("fork failed! [%d]\n", pid);
		case 0:	// Child process
			child_process();
			exit(EXIT_SUCCESS);
		default:	// Parent process
			waitpid(pid, &status, 0);
		}
	}
	exit(EXIT_SUCCESS);
}

