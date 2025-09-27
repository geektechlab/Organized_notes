#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

void signal_handler(int signum)
{
	printf("%s\n", __func__);
}

int main(void)
{
	printf("Pausing until we receive a SIGINT signal (Ctrl+C).\n");
	printf("This process has pid %d\n", getpid());

	signal(SIGINT, signal_handler);
	sigset_t set;
	sigfillset(&set);
	sigdelset(&set, SIGINT);

	/* sigsuspend() will set the signal mask to the set temporarily. If a
	   signal is received:
	   - If that signal would result in process termination, the
          process is terminated without sigsuspend() returning.
	   - If that signal invokes a signal handler, sigsuspend() returns
          after the signal handler completes.
	   The signal mask is restored when sigsuspend() returns.
	*/
	
	errno = 0;
	sigsuspend(&set); // always returns -1, may set errno
	if(errno == EFAULT)
	{
		perror("sigsuspend:");
		exit(EXIT_FAILURE);
	}

	printf("Returned from sigsuspend(), exiting.\n");
	return 0;
}

