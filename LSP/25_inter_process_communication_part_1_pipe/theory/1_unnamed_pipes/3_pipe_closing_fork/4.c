//If all file descriptors referring to the read end of a pipe have been closed, then a write(2) will cause a SIGPIPE signal to be generated for the calling process. 

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

void sighandler(int signum)
{
	printf("signal handler:%d\n", signum);
}

int main()
{
	int filedes[2];
	char buffer[] = "hello pipe";
	char readbuffer[15] = {};
	pid_t childpid;

	signal(SIGPIPE, sighandler);

	pipe(filedes);

	close(filedes[0]);
	printf("Parent writes:%s\n", buffer);
	write(filedes[1], buffer, sizeof(buffer));
	close(filedes[1]);

	return 0;
}
