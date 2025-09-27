#include <sys/stat.h>
#include <stdio.h>

/* run [ ./filetype <files mentioned in postnotes.txt> ] and what it gives */

int main(int argc, char *argv[])
{
	int i;
	struct stat statbuf;
	char *ptr;

	for (i = 1; i < argc; ++i) {
		printf("%s: ", argv[i]);
		if (lstat(argv[i], &statbuf) < 0) {
			printf("%s: lstat error", argv[0]);
			continue;
		}

		if (S_ISREG(statbuf.st_mode))
			ptr = "regular";
		else if (S_ISDIR(statbuf.st_mode))
			ptr = "directory";
		else if (S_ISLNK(statbuf.st_mode))
			ptr = "symbolic link";
		else if (S_ISBLK(statbuf.st_mode))
			ptr = "block special";
		else if (S_ISCHR(statbuf.st_mode))
			ptr = "character special";
		else if (S_ISSOCK(statbuf.st_mode))
			ptr = "socket";
		else if (S_ISFIFO(statbuf.st_mode))
			ptr = "FIFO/pipe";
		else
			ptr = "** unknown mode **";
		printf("%s\n", ptr);
	}
	exit(0);
}
