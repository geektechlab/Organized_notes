//To and fro communication between child

/* for full duplex cmmunicxation, we need 2 pipes. With 1 pipe, only one direction communication can happen */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int filedes[2];
	char writebuffer[] = "hello pipe";
	char readbuffer[15] = {};
	pid_t childpid;

	pipe(filedes);
	perror("pipe");

	childpid = fork();
	if (childpid == 0) {
		read(filedes[0], readbuffer, sizeof(readbuffer));
		printf("child read:%s\n", readbuffer);
		memset(writebuffer, 0, sizeof(writebuffer));
		strcpy(writebuffer, "hello parent");
		printf("Child writes:%s\n", writebuffer);
		write(filedes[1], writebuffer, sizeof(writebuffer));
		close(filedes[0]);
		close(filedes[1]);
	} else {
		printf("Parent writes:%s\n", writebuffer);
		write(filedes[1], writebuffer, sizeof(writebuffer));
		memset(readbuffer, 0, sizeof(readbuffer));
		read(filedes[0], readbuffer, sizeof(readbuffer));
		printf("Parent reads:%s\n", readbuffer);
		close(filedes[0]);
		close(filedes[1]);
		wait(NULL);
	}

	return 0;
}
