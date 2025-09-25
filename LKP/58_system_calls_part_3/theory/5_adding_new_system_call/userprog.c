#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/syscall.h>
#include <string.h>

/* check in [ vi arch/x86/entry/syscalls/syscall_64.tbl ] that 434 entry is added ( hello system call ) */
/* check in dmesg because this system call writes in kernel space */

int main(void)
{
    long return_value = syscall(434);

    printf("return value from syscall: %ld, erron:%d\n", return_value, errno);
    return 0;
}
