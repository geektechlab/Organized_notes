//To and fro communication between child and parent
//If you need to send data to and from a child asynchronously, then two pipes are required (one for each direction).Otherwise the child would attempt to read its own data intended for the parent (and vice versa)!

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int filedes1[2], filedes2[2];
	char writebuffer[30] = "hello child";
	char readbuffer[15] = {};
	pid_t childpid;

	pipe(filedes1);
	perror("pipe");
	pipe(filedes2);
	perror("pipe");

	childpid = fork();
	if (childpid == 0) {
		read(filedes1[0], readbuffer, sizeof(readbuffer));
		printf("child read:%s\n", readbuffer);
		memset(writebuffer, 0, sizeof(writebuffer));
		strcpy(writebuffer, "hello parent");
		printf("Child writes:%s\n", writebuffer);
		write(filedes2[1], writebuffer, sizeof(writebuffer));
		close(filedes1[0]);
		close(filedes1[1]);
		close(filedes2[0]);
		close(filedes2[1]);
	} else {
		printf("Parent writes:%s\n", writebuffer);
		write(filedes1[1], writebuffer, sizeof(writebuffer));
		memset(readbuffer, 0, sizeof(readbuffer));
		read(filedes2[0], readbuffer, sizeof(readbuffer));
		printf("Parent reads:%s\n", readbuffer);
		close(filedes1[0]);
		close(filedes1[1]);
		close(filedes2[0]);
		close(filedes2[1]);
		wait(NULL);
	}

	return 0;
}
