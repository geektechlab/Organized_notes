#define _GNU_SOURCE  
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define STACK_SIZE	65536

int catch_signal;
struct sigaction sa;

static void sighandler(int sig)
{
	printf("Caught signal %d (%s) \n", sig, strsignal(sig));
}

static int child_func(void *arg)
{
	signal(catch_signal, sighandler);
	kill(getpid(), SIGTERM);
	return 0;
}

int main(int argc, char *argv[])
{
	//Allocate stack for child task
	char *stack = malloc(STACK_SIZE);
	unsigned long flags = 0;
	char buf[256];
	int status;

	catch_signal = SIGTERM;
	if (signal(catch_signal, SIG_IGN) == SIG_ERR) {
	      perror("signal");
	      exit(1);
	}
	if (!stack) {
		perror("Failed to allocate memory\n");
		exit(1);
	}

	/* in this case CLONE_SIGHAND will indicate signal handlers will be shared */
	if (argc == 2 && !strcmp(argv[1], "signal"))
		flags |= CLONE_SIGHAND; 
	if (clone(child_func, stack + STACK_SIZE, flags | CLONE_VM |SIGCHLD, NULL) == -1) {
		perror("clone");
		exit(1);
	}

	if (wait(&status) == -1) {
		perror("Wait");
		exit(1);
	}	

	printf("Child exited with status:%d\n", status);
	kill(getpid(), SIGTERM);
	return 0;
}
