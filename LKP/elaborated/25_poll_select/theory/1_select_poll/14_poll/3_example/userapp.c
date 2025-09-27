#include <stdio.h>
#include <sys/poll.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd;
    char buf[1024];    
    struct pollfd fds[1];

    fd = open("/dev/mydevice", O_RDWR);

    fds[0].fd = fd;
    fds[0].events = POLLIN;

    //Specifying a negative  value in  timeout  means  an  infinite timeout.
    poll(fds, 1, -1);
    perror("poll");

    if (fds[0].revents & POLLIN) {
        printf("data available\n");
        memset(buf, 0, sizeof(buf));
        read(fd, buf, sizeof(buf));
        printf("data:%s\n", buf);
    }
    else
        printf("data not available\n");

	return 0;
}
