#include <stdio.h>
#include <fcntl.h>

int main()
{
	char buf[100];
	ssize_t write_retval;
	int fd = open("hello.txt", O_WRONLY);
	if (fd < 0) {
		perror("open failed");
	} else {
		printf("Opening file successful\n");
		printf("Enter the Data to be written into hello.txt:");
		scanf("%[^\n]s", buf);
		//scanf("%s", buf);
		write_retval = write(fd, buf, strlen(buf));
		printf("write_retval:%d\t strlen(buf):%d\n", write_retval, strlen(buf));
		/* if we don't close file then process may not be able to open file if limit is excedded. File opening limit is set by [ ulimit -a ]. So closing file is recommeded.
		//close(fd);
	}
	return 0;
}
