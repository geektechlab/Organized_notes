#include <signal.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <limits.h>
#include <pthread.h>

int count = 0;

/* Iterates through a list of signals and prints out which signals are in a signal set. */
void printSignalSet(sigset_t *set)
{
	/* This listing of signals may be incomplete. */
	const int sigList[] = { SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGTRAP, 
		SIGABRT, SIGBUS, SIGFPE, SIGKILL, SIGUSR1, SIGSEGV, SIGUSR2, SIGPIPE,
		SIGALRM, SIGTERM, SIGSTKFLT,
		SIGCHLD, SIGCONT, SIGSTOP,
		SIGTSTP, SIGTTIN, SIGTTOU };
	const char *sigNames[] = { "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP",
		"SIGABRT", "SIGBUS", "SIGFPE", "SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2",
		"SIGPIPE", "SIGALRM",  "SIGTERM", "SIGSTKFLT", 
		"SIGCHLD", "SIGCONT", "SIGSTOP",
		"SIGTSTP", "SIGTTIN", "SIGTTOU" };
	const int sigLen = 22;

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

void *threadfunc(void *arg)
{
	sigset_t set;
	printf("In thread function\n");
	if(sigprocmask(0, NULL, &set) != 0)
	{
		perror("sigprocmask:");
		exit(EXIT_FAILURE);
	}

	printSignalSet(&set);

	printf("Completed thread function\n");
	pthread_exit(NULL);
}

int main(){
	sigset_t set, newset;
	pthread_t tid;

	sigemptyset(&newset);
	sigaddset(&newset, SIGUSR1);
	sigprocmask(SIG_BLOCK, &newset, NULL);

	if(sigprocmask(0, NULL, &set) != 0)
	{
		perror("sigprocmask:");
		exit(EXIT_FAILURE);
	}

	printSignalSet(&set);

	pthread_create(&tid, NULL, threadfunc, NULL);
	pthread_exit(NULL);
}
