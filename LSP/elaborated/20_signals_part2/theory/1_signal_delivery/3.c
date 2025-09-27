#include <stdio.h>
#include <signal.h>


void sighandler(int signum)
{
	printf("%s\n", __func__);
	exit(1);
}

int main()
{
	int a = 5;
	signal(SIGFPE, sighandler);
	a = 5/0;
	return 0;
}
