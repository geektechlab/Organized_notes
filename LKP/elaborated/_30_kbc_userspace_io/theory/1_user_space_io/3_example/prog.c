#include <stdio.h>
#include <errno.h>
#include <sys/io.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	//Allow only root user to run this program, uid of root is 0
	if (geteuid() != 0) {
		fprintf(stderr, "Must be root to run %s\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (ioperm(0x70, 3, 1)) {
		perror("ioperm failed");
	}
	else {
		printf("ioperm on 0x70 success\n");
	}

	return 0;
}
