#include <unistd.h>
#include <stdio.h>

/* just a self-query, run [ proc ] etc. to find both processes's memory map */

int globval = 6;		/* external variable in initialized data */
char buf[] = "a write to stdout\n";

int main(void)
{
	int autoval;		/* automatic variable on the stack */
	pid_t pid;

	autoval = 88;
	if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1)
		printf("write %s to stdout error\n", buf);
	printf("before fork\n");	/* we don't flush stdout */

	if ((pid = fork()) < 0) {
		printf("fork error\n");
		return 1;
	}
	else if (pid == 0)	{	/* child */
		/* for child, these values will get changed */
		/* because of COW, this section's page will get changed */
		++globval;			/* modity variables */
		++autoval;
	}
	else					/* parent */
		sleep(2);

	/* but for text section, there is no change, so printf related page won't get changed */
	printf("pid = %d, global = %d, autoval = %d\n", 
			getpid(), globval, autoval);
	return 0;
}
