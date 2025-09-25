#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/* Iterates through a list of signals and prints out which signals are in a signal set. */
void printSignalSet(sigset_t *set)
{
	/* This listing of signals may be incomplete. */
	const int sigList[] = { SIGHUP, SIGINT, SIGQUIT, SIGILL,
	                        SIGABRT, SIGFPE, SIGKILL, SIGSEGV,
	                        SIGPIPE, SIGALRM, SIGTERM, SIGUSR1,
	                        SIGUSR2, SIGCHLD, SIGCONT, SIGSTOP,
	                        SIGTSTP, SIGTTIN, SIGTTOU };
	const char *sigNames[] = { "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL",
	                           "SIGABRT", "SIGFPE", "SIGKILL", "SIGSEGV",
	                           "SIGPIPE", "SIGALRM", "SIGTERM", "SIGUSR1",
	                           "SIGUSR2", "SIGCHLD", "SIGCONT", "SIGSTOP",
	                           "SIGTSTP", "SIGTTIN", "SIGTTOU" };
	const int sigLen = 19;
	
	for(int i=0; i<sigLen; i++)
	{
		int ret = sigismember(set, sigList[i]);
		if(ret == -1)
		{
			perror("sigismember:");
			exit(EXIT_FAILURE);
		}
		else if(ret == 1)
		{
			printf("Signal %s=%d IS in the set.\n", sigNames[i], sigList[i]);
		}
		else
		{
			printf("Signal %s=%d is not in the set.\n", sigNames[i], sigList[i]);
		}
	}
}

int main(void)
{
	sigset_t set;

	if(sigprocmask(0, NULL, &set) != 0)
	{
		perror("sigprocmask:");
		exit(EXIT_FAILURE);
	}

	printSignalSet(&set);

		if(sigaddset(&set, SIGINT) != 0) // Add SIGINT to our set
	{
		perror("sigaddset:");
		exit(EXIT_FAILURE);
	}
	/* Tell OS that we want to mask our new set of signals---which now includes SIGINT. */
	if(sigprocmask(SIG_SETMASK, &set, NULL) != 0)
	{
		perror("sigprocmask:");
		exit(EXIT_FAILURE);
	}

	/* Now, SIGINT will be "blocked". */
	printf("--- NEW signal mask for this process: ---\n");
	printSignalSet(&set);

	int secToSleep = 20;
	printf("Try Ctrl+C. Try killing this program with 'kill -kill %d'. Going to sleep for %d seconds.\n", getpid(), secToSleep);
	sleep(secToSleep);
	printf("Sleep returned.\n");


	/* Look for any signals that are currently blocked---and would be
	 * triggered once they are unmasked. */
	sigset_t pendingSignalSet;
	sigpending(&pendingSignalSet);
	printf("--- Signals which are blocked/pending: ---\n");
	printSignalSet(&pendingSignalSet);

	/* If SIGINT was blocked and the user pressed Ctrl+C during the
	 * sleep above, that SIGINT signal is just blocked. By unmasking
	 * that signal, the blocked SIGINT will be delivered. */
	printf("Removing all signals from mask.\n");
	if(sigemptyset(&set) != 0)
	{
		perror("sigemptyset:");
		exit(EXIT_FAILURE);
	}
	if(sigprocmask(SIG_SETMASK, &set, NULL) != 0)
	{
		perror("sigprocmask:");
		exit(EXIT_FAILURE);
	}

	/* If you pressed Ctrl+C during the sleep, the SIGINT should be
	 * processed here. This means that the following message wouldn't
	 * be printed at all because SIGINT would cause the program to
	 * exit instead of us reaching the end of main() */
	sleep(1);
	printf("Exited normally.\n");
	return 0;
}
