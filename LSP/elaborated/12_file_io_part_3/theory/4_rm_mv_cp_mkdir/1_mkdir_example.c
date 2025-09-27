#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define DIR_TO_MAKE "mkdir-example.temp"

int main(void)
{
	// The second argument to mkdir() is a number. Typically
	// permissions are written in the octal numbering system. To write
	// an octal number and have it be interpreted correctly by the
	// compiler, you need to put a leading 0 in front of it. For
	// example, if you want to create a directory named "mkdir.temp"
	// where the owner has read/write/execute permissions on it, you
	// would do this:
	/* run [ man mkdir ] for more details */
	/* here first 0 in 0700 signifies octal number system */
	/* after running this, do [ ls -lh mkdir-example.temp ] to check permissions */
	/* mkdir is system call, verify it by [ strace mkdir_example ] */
	if(mkdir(DIR_TO_MAKE, 0700) == -1)
	{
		printf("Failed to create %s\n", DIR_TO_MAKE);
		perror("mkdir");
		exit(EXIT_FAILURE);
	}
	else
		printf("Created directory: %s\n", DIR_TO_MAKE);
}
