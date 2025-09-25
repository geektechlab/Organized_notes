#include <stdio.h>
#include <sys/poll.h>

int main()
{
    
    struct pollfd fds[1];

    fds[0].fd = fileno(stdin);
    fds[0].events = POLLIN;

    //Specifying a negative  value in  timeout  means  an  infinite timeout.
    poll(fds, 1, -1);
    perror("poll");

    if (fds[0].revents & POLLIN)
        printf("data available on stdin\n");
    else
        printf("data not available on stdin\n");

	return 0;
}
