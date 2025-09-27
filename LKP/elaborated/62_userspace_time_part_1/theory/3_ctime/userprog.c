#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char argv[])
{
    time_t epoch_time;

    time(&epoch_time);
    printf("Time %s\n", ctime(&epoch_time));
    return 0;
}

