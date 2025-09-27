#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char argv[])
{
    struct timespec walltime1;
    struct timeval  walltime2;

    clock_gettime(CLOCK_REALTIME, &walltime1);
    gettimeofday(&walltime2, NULL);

    printf("wall time1:%s\n", ctime(&walltime1.tv_sec));
    printf("wall time2:%s\n", ctime(&walltime2.tv_sec));
    return 0;
}

