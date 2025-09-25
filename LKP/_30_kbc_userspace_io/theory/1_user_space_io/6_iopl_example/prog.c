#include <stdio.h>
#include <errno.h>
#include <sys/io.h>

int main(int argc, char *argv[])
{
	if (iopl(3)) {
		perror("iopl failed");
	}
	else {
		printf("iopl success\n");
	}

	return 0;
}
