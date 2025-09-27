#include <stdio.h>
#include <fcntl.h>

int main()
{
	char buf[100];
	ssize_t write_retval;
	/* if we have some content already in file, if we don't add O_APPEND in open call then open will give descriptor from start and then write may use that descriptor and might overwrite content from start */
	/* try having some file with content and then try without O_APPEND */
	/* if we want to remove entire previous content without just oerlap replacement then first we can pass truncate argument in open and then do later operations */
	int fd = open("hello.txt", O_WRONLY|O_APPEND);
	if (fd < 0) {
		perror("open failed");
	} else {
		printf("Opening file successful\n");
		printf("Enter the Data to be written into hello.txt:");
		scanf("%[^\n]s", buf);
		write_retval = write(fd, buf, strlen(buf));
		printf("write_retval:%d\t strlen(buf):%d\n", write_retval, strlen(buf));
		close(fd);
	}
	return 0;
}
