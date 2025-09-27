#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/syscall.h>
#include <string.h>

/* here we are passing 257 instead of 256 acceptable size of buffer, so errornum will get updated */

int main(void)
{
	char buf[257];

	long return_value;

	memset(buf, 'a', sizeof(buf));
	return_value = syscall(435, buf);

	printf("return value from syscall: %ld, erron:%d\n", return_value, errno);
	return 0;
}
