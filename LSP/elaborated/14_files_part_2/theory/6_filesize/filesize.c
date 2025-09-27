#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	struct stat buf;

	if (argc != 2) {
		printf("Usage: %s <pathname>", argv[0]);
		exit(1);
	}

	if (lstat(argv[1], &buf) < 0)
		printf("%s: stat %s error", argv[0], argv[1]);

	printf("The size of %s is: %lld\n", argv[1], (long long)buf.st_size);

	exit(0);
}
