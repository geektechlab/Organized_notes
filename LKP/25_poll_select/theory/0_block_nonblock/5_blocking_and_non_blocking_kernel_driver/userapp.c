#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	char buffer[10];
	int fd = open("/dev/mydevice", O_RDWR|O_NONBLOCK);
	read(fd, buffer, sizeof(buffer));
	perror("read");
	close(fd);
	return 0;
}
