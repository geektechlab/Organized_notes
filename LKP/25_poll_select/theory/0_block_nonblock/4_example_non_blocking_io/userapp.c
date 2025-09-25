#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	char buffer[10];
	int fd = open("/dev/mydevice", O_RDWR|O_NONBLOCK);
	read(fd, buffer, sizeof(buffer));
	write(fd, "hello", sizeof("hello"));
	close(fd);
	return 0;
}
