#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void sighandler(int signo)
{
	printf("In signal handler.\n");
}

int main(void)
{
	printf("Pausing until we receive a signal.\n");
	printf("This process has pid %d\n", getpid());
	signal(SIGINT, sighandler);
	pause();
	printf("Returned from pause(), exiting.\n");
	return 0;
}
