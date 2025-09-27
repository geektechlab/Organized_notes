#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>

int main()
{   
    int retval;
    char buffer[10];
    fd_set readset, writeset, exceptset;
    int fd = open("/dev/mydevice", O_RDWR);
    perror("open");

    FD_ZERO(&readset);
    FD_ZERO(&writeset);
    FD_ZERO(&exceptset);

    printf("fd of /dev/mydevice:%d\n", fd);
    FD_SET(fd, &readset);

    printf("calling select\n");
    retval = select(fd + 1, &readset, &writeset, &exceptset, NULL);
    printf("select returned:%d\n", retval);

    if (FD_ISSET(fd, &readset))
    {
        printf("calling read\n");
        retval = read(fd, buffer, sizeof(buffer));
        buffer[retval] = '\0';
        printf("buffer:%s\n", buffer);
    }

    close(fd);
}   
