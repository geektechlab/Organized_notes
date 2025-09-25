#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char *addr;
	int fd;
	struct stat sb;

	if (argc != 2 || strcmp(argv[1], "--help") == 0)
		printf("%s file\n", argv[0]);

	fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(-1);
	}

	fstat(fd, &sb);

	addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	write(STDOUT_FILENO, addr, sb.st_size);
	exit(EXIT_SUCCESS);
}
