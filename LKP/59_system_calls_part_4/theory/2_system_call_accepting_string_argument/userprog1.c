#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/syscall.h>
#include <string.h>

/* to inject custom system call, we have to recompile kernel and install it ( watch video 5-10 ) */

int main(void)
{
    long return_value = syscall(435, "hello");

    printf("return value from syscall: %ld, erron:%d\n", return_value, errno);
    return 0;
}
