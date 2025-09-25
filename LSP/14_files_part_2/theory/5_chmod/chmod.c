#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void)
{
	struct stat statbuf;
	int fd;

	/* For the file foo, we set the permissions relative to their current
	 * state. To do this, we first call stat() to obtain the current
	 * permissions and then modify them.
	 */
	if (stat("foo", &statbuf) < 0) {
		printf("stat foo error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0) {
		printf("chmod foo error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if ((fd = open("bar", O_RDWR)) < 0) {
		printf("open bar error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* We have set the permissions of the file bar to an absolute value,
	 * regardless of the current permission bits.
	 */
	if (fchmod(fd, S_IRUSR | S_IWUSR | S_IWGRP | S_IWOTH) < 0) {
		printf("chmod bar error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
