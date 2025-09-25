#include <stdio.h>
#include <fcntl.h>

/* change to /tmp and open given file as relative path */

int main()
{	
	int fd ;
	if (!chdir("/tmp"))
	{
		printf("Changed to tmp directory\n");
	}
	else
	{
		perror("Failed to change to tmp directory");
	}

	fd = openat(AT_FDCWD, "hello.txt", O_RDONLY|O_CREAT);

	if (fd < 0)
	{
		perror("Openat failed");
	}
	else
	{
		printf("Openat successful\n");
		close(fd);
	}
	return 0;
}
