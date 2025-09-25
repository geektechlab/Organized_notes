#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/syscall.h>
#include <string.h>

/* running by building wth gcc -m32 flag will generate code for the i386 ABI and it will fail
gcc -mx32 flag will generate code for the x32 ABI and it will pass */

int main(void)
{
    long return_value = syscall(434);

    printf("return value from syscall: %ld, erron:%d\n", return_value, errno);
    return 0;
}
