#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

int main()
{
    int fd;
	/* passing O_DIRECTORY is required for opening directory else it will fail */
    if ((fd = open("/home/panther2", O_DIRECTORY | O_RDONLY)) ==-1)
	/* we can open a directory only in read mode, not in write mode. following call will fail */
    //if ((fd = open("/home/panther2",  O_WRONLY)) ==-1)
    {
        printf("error %s\n", strerror(errno));
       return -1;
    }
    return 0;
}
