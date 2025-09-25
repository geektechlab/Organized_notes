#include <unistd.h>
#include <stdio.h>

/* when we run ./pid, parent process is shell in which we run this. getppid() prints it. We can verify it by [ ps ] command. */

int main(void)
{
	printf("process ID: %d\n", getpid());
	printf("parent process ID: %d\n", getppid());
	return 0;
}
