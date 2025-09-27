#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> /* access() */
#include <fcntl.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Determines whether real IDs has read access to the file */
	if (access(argv[1], R_OK) < 0)
		printf("%s: access %s for R_OK error: %s\n", argv[0],
				argv[1], strerror(errno));
	else
		printf("%s: access %s for R_OK success\n", argv[0], argv[1]);

	/* Determine if the effective IDs of the process can open the file in read-only mode */
	if (open(argv[1], O_RDONLY) < 0)
		printf("%s: open %s for O_RDONLY error: %s\n", argv[0],
				argv[1], strerror(errno));
	else
		printf("%s: open %s for O_RDONLY success\n", argv[0], argv[1]);
	
	/* Determines if read IDs has write access to the file */
	if (access(argv[1], W_OK) < 0)
		printf("%s: access %s for W_OK error: %s\n", argv[0],
				argv[1], strerror(errno));
	else
		printf("%s: access %s for W_OK success\n", argv[0], argv[1]);

	/* Determines if read IDs have executable permissions on the file */
	if (access(argv[1], X_OK) < 0)
		printf("%s: access %s for X_OK error: %s\n", argv[0],
				argv[1], strerror(errno));
	else
		printf("%s: access %s for X_OK success\n", argv[0], argv[1]);

	/* Determine if a file exists */
	if (access(argv[1], F_OK) < 0)
		printf("%s: file %s does not exist \n", argv[0], argv[1]);
	else
		printf("%s: file %s exists\n", argv[0], argv[1]);

	exit(EXIT_SUCCESS);
}
