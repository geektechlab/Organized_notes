#include <stdio.h>
#include <sys/poll.h>
#include <string.h>
#include <unistd.h>

int main()
{
    char buf[1024];    
    struct pollfd fds[1];

    fds[0].fd = fileno(stdin);
    fds[0].events = POLLIN;

    //Specifying a negative  value in  timeout  means  an  infinite timeout.
    poll(fds, 1, -1);
    perror("poll");

    if (fds[0].revents & POLLIN) {
        printf("data available on stdin\n");
        memset(buf, 0, sizeof(buf));
        read(0, buf, sizeof(buf));
        printf("data:%s\n", buf);
    }
    else
        printf("data not available on stdin\n");

	return 0;
}
