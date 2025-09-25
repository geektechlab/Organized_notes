#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define FIFO_NAME "/tmp/my_fifo"

int main()
{
	int fd;
	int result;

	char buffer[10] = "Linux";

	if (access(FIFO_NAME, F_OK) == -1) {
		result = mkfifo(FIFO_NAME, 0777);
		if (result != 0) {
			fprintf(stderr, "Could not create fifo %s\n", FIFO_NAME);
			exit(EXIT_FAILURE);
		}
	}

	printf("Process %d opening FIFO\n", getpid());

	fd = open(FIFO_NAME, O_RDONLY );
	if (fd < 0) {
		perror("open");
	} else {
		int ret;
		printf("opened fifo successfully\n");
		ret = read(fd, buffer, sizeof(buffer));
		perror("read");
		printf("read :%d bytes\t data:%s\n", ret, buffer);
		close(fd);
	}
	printf("Process %d finished\n", getpid());
	exit(EXIT_SUCCESS);
}
