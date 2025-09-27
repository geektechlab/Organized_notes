#include <stdio.h>
#include <unistd.h>

int main()
{
	int filedes[2];

	if (pipe(filedes) == 0) {
		printf("Pipe successful\n");
		printf("read from %d, write to %d\n", filedes[0], filedes[1]);
	} else {
		perror("pipe failed");
	}
	return 0;
}
