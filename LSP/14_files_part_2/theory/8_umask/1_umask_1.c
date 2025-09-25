#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#define RWRWRW	(S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

int main(int argc, char *argv[])
{
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	if (creat("bar", RWRWRW) < 0) {
		printf("%s: creat bar error:%s\n",argv[0],strerror(errno));
		exit(EXIT_FAILURE);
	}

	umask(0);
	if (creat("foo", RWRWRW) < 0) {
		printf("%s: creat foo error:%s\n",argv[0],strerror(errno));
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
