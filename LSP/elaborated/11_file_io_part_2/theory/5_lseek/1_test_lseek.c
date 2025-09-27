#include <stdio.h>
#include <fcntl.h>

// /* we are reading file offset and using lseek we update file's offset */

int main()
{
	//int fd = open("hello.txt", O_RDWR);
	/* following call will fail because file will get opened in write mode only. We can verify it using perror */
	//int fd = open("hello.txt", O_WRONLY | O_APPEND);
	int fd = open("hello.txt", O_RDONLY);
	char buf[10];
	off_t curpos;
	if (fd < 0) {
		perror("open failed");
	} else {
		printf("Opening file successful\n");
		//we can seek zero bytes from the current position to determine the current offset
		curpos = lseek(fd, 0, SEEK_CUR);
		printf("curpos:%lu\n", curpos);
		/* it will update file's offest */
		read(fd, buf, sizeof(buf));
		perror("read operation");
		printf("Buffer:%s\n", buf);
		/* we are setting file offset + 0 */
		curpos = lseek(fd, 0, SEEK_CUR);
		printf("curpos:%lu\n", curpos);
		/* it will update file's offest */
		read(fd, buf, sizeof(buf));
		perror("read operation");
		printf("Buffer:%s\n", buf);
		/* we are setting file offset + 0 */
		curpos = lseek(fd, 0, SEEK_CUR);
		printf("curpos:%lu\n", curpos);
		close(fd);
	}
	return 0;
}
