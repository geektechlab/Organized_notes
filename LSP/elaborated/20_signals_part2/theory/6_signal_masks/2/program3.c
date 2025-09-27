#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main()
{
	int ret;

	sigset_t newset, oldset;

	sigemptyset(&newset);
	sigaddset(&newset, SIGINT);
	sigprocmask(SIG_SETMASK, &newset, &oldset);

	ret = execl("/home/linuxtrainer/Linux_System_Prog/day10_signals/14/1/2", "2", NULL);
	if (ret == -1)
		perror("execl");
}
