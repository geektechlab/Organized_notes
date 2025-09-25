#include <stdio.h>
#include <fcntl.h>

int main()
{
	char seconds=0;
	char data = 0;
	int fd = open("/dev/port", O_RDWR);

	perror("open");
	
	lseek(fd, 0x70, SEEK_SET);
	write(fd, &data, 1);

	lseek(fd, 0x71, SEEK_SET);
	read(fd, &seconds, 1);
	printf("%02X ", seconds);


	close(fd);
	return 0;
}
