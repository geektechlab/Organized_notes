#include <stdio.h>
#include <errno.h>
#include <sys/io.h>

int main(int argc, char *argv[])
{
	if (ioperm(0x70, 3, 1)) {
		perror("ioperm failed");
	}
	else {
		printf("ioperm on 0x70 success\n");
	}

	return 0;
}
