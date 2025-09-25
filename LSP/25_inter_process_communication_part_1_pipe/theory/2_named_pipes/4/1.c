#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#define FIFO_NAME "/tmp/my_fifo"

int main()
{
	int fd;
	int result;

	if (access(FIFO_NAME, F_OK) == -1) {
		result = mkfifo(FIFO_NAME, 0777);
		if (result != 0) {
			fprintf(stderr, "Could not create fifo %s\n", FIFO_NAME);
			exit(EXIT_FAILURE);
		}
	}

	printf("Process %d opening FIFO\n", getpid());

	fd = open(FIFO_NAME, O_RDWR);
	if (fd < 0) {
		perror("open");
	} else {
		close(fd);
	}
	printf("Process %d finished\n", getpid());
	exit(EXIT_SUCCESS);
}
