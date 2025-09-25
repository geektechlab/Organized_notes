#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char argv[])
{
    struct timespec uptime;

    clock_gettime(CLOCK_MONOTONIC, &uptime);
    perror("clock_gettime");
    printf("Current up time:%lu\n", uptime.tv_sec);

    uptime.tv_sec += 10*60;
    printf("Setting up time to :%lu\n", uptime.tv_sec);
    clock_settime(CLOCK_MONOTONIC, &uptime);
    perror("clock_settime");

    clock_gettime(CLOCK_MONOTONIC, &uptime);
    perror("clock_gettime");
    printf("Current up time:%lu\n", uptime.tv_sec);

    return 0;
}

