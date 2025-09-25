#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char argv[])
{
    struct timespec walltime;

    clock_gettime(CLOCK_REALTIME, &walltime);
    perror("clock_gettime");
    printf("Current wall time:%s\n", ctime(&walltime.tv_sec));

    walltime.tv_sec += 10*60;
    printf("Setting wall time to :%s\n", ctime(&walltime.tv_sec));
    clock_settime(CLOCK_REALTIME, &walltime);
    perror("clock_settime");

    clock_gettime(CLOCK_REALTIME, &walltime);
    perror("clock_gettime");
    printf("Current wall time:%s\n", ctime(&walltime.tv_sec));

    return 0;
}

