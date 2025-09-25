#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char argv[])
{
    struct timeval start, end;
    unsigned long elapsed_seconds = 0;
    unsigned long elapsed_microseconds = 0;

    gettimeofday(&start, NULL);

    getchar();

    gettimeofday(&end, NULL);
    elapsed_seconds = end.tv_sec - start.tv_sec;
    elapsed_microseconds = end.tv_usec - start.tv_usec;
    printf("Time elapsed is %ld seconds and %ld microseconds\n", elapsed_seconds, elapsed_microseconds);
    return 0;
}
