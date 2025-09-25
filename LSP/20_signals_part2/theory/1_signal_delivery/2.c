#include <stdio.h>
#include <signal.h>

/* it will continously go to sighandler */

void sighandler(int signum)
{
	printf("%s\n", __func__);
	exit(1);
}

int main()
{
	int *a = NULL;
	signal(SIGSEGV, sighandler);
	*a = 5;
	return 0;
}
