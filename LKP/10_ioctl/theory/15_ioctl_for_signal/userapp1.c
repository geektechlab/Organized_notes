#include <stdio.h>
#include <fcntl.h> 
#include <stdlib.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <unistd.h>
#include "ioctl_cmd.h"

void signal_handler(int sig)
{
	printf("Signal Received\n");
}

int main(int argc, char *argv[])
{
	int fd;
	pid_t pid = getpid();
	int snd_signal = SIGUSR1;

	printf("PROCESS PID:%d\n", pid);

	signal(SIGUSR1, signal_handler);
	fd = open("/dev/sig", O_RDWR);
	if (fd < 0) {
		perror("fd failed");
		exit(2);
	}

	ioctl(fd, SIG_IOCTL_SET_PID, &pid);
	ioctl(fd, SIG_IOCTL_SET_SIGNAL, &snd_signal);
	ioctl(fd, SIG_IOCTL_SEND_SIGNAL);
	perror("ioctl");
	getchar();

	close(fd);
}
