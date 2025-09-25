#include <unistd.h>
#include <stdio.h>

/* watch https://www.youtube.com/watch?v=lp7FgLR2qwQ from 9:49 to 22:00 */

int main(void)
{
	printf("process ID: %d\n", getpid());
	printf("parent process ID: %d\n", getppid());
	printf("real user ID: %d\n", getuid());
	printf("effective user ID: %d\n", geteuid());
	printf("real group ID: %d\n", getgid());
	printf("effective group ID: %d\n", getegid());

	return 0;
}
