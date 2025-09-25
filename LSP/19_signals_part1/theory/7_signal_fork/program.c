#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void term_signal(int signum)
{
	printf("PID:%u\n", getpid());
}

int main() {
	pid_t pid;
	signal(SIGINT, term_signal);
	printf("My pid is %d\n", getpid());
	pid = fork();
	if ( pid == 0) {
		int i = 60;
		while (--i) { 
			write(1, ".", 1);
			sleep(1);
		}
		write(1, "Done!", 5);
	} else {
		int i = 5;
		printf("Parent process running\n");
		while(--i) {
			kill(pid, SIGSTOP);
			sleep(4);
			kill(pid, SIGCONT);
			sleep(4);
		}
	}
	return 0;
}
