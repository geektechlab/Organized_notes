#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
	int filedes[2];
	char buffer[] = "hello pipe";
	char readbuffer[15] = {};
	pid_t childpid;

	pipe(filedes);
	perror("pipe");

	childpid = fork();
	if (childpid == 0) {
		read(filedes[0], readbuffer, sizeof(readbuffer));
		printf("child read:%s\n", readbuffer);
		close(filedes[0]);
		close(filedes[1]);
	} else {
		printf("Parent writes:%s\n", buffer);
		write(filedes[1], buffer, sizeof(buffer));
		close(filedes[0]);
		close(filedes[1]);
		wait(NULL);
	}

	return 0;
}
