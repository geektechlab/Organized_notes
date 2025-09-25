#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char argv[])
{
    struct timespec walltime;

    clock_gettime(CLOCK_REALTIME, &walltime);
    printf("wall time:%s\n", ctime(&walltime.tv_sec));
    return 0;
}

