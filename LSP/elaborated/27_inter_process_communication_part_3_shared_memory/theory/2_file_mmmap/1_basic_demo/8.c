#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char *srcaddr, *dstaddr;
	int fdsrc, fddst;
	struct stat sb;

	if (argc != 3)
		printf("%s source-file dest-file\n", argv[0]);

	fdsrc = open(argv[1], O_RDONLY);
	fddst = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

	if (fdsrc == -1 || fddst == -1) {
		perror("open");
		exit(-1);
	}

	fstat(fdsrc, &sb);

	srcaddr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fdsrc, 0);
	ftruncate(fddst, sb.st_size);
	dstaddr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fddst, 0);	

	memcpy(dstaddr,  srcaddr, sb.st_size); 
	exit(EXIT_SUCCESS);
}
