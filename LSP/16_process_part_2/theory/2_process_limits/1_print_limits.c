#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>

/* linux command for doing same : [ ulimit -a ] */

#define doit(name) pr_limits(#name, name)
static void pr_limits(char *, int);

int main(void)
{
	//The maximum number of open files per process.
	doit(RLIMIT_NOFILE);

	//The maximum size in bytes of the stack.
	doit(RLIMIT_STACK);

	//The maximum amount of memory in bytes that a process can allocate for POSIX message queues
	doit(RLIMIT_MSGQUEUE);

	return 0;
}



static void pr_limits(char *name, int resource)
{
	struct rlimit limit;
	unsigned long long lim;
	if (getrlimit(resource, &limit) < 0)
		printf("getrlimit error for %s", name);
	printf("%-14s ", name);
	if (limit.rlim_cur == RLIM_INFINITY) {
		printf("(infinite) ");
	} else {
		lim = limit.rlim_cur;
		printf("%10lld ", lim);
	}
	if (limit.rlim_max == RLIM_INFINITY) {
		printf("(infinite)");
	} else {
		lim = limit.rlim_max;
		printf("%10lld", lim);
	}
	putchar('\n');
}
