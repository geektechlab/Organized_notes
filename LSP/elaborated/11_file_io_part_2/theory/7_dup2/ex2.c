#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    int fd, assignvalue, dup2fd, fd2;
    int n;
    off_t offset;
    char buf[64];

    if ((fd2 = open("test", O_RDWR | O_CREAT, 0644)) < 0) {
        printf("dup: can't open the file (test): %m\n");
        exit(1);
    }

	if (dup2(fd2, 5) < 0) {
        printf("dup: can't dup2 fd: %d: %m\n", fd2);
        exit(1);
    }
    if (write(5, "tianxia\n", 8) != 8) {
        printf("dup: can't write to %d: %s\n", fd, strerror(errno));
        exit(1);
    }

	assignvalue = 5;
	 close(fd2);
    if (write(assignvalue, "tian\n", 5) != 5) {   
        printf("dup: can't write to assignvalue: %d: %m\n", assignvalue);
    }
    exit(0);
	
}
