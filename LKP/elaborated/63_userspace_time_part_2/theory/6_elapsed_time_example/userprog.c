#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

/* elapsed time to do j = i/256; and j = i >> 8; */

int main(int argc, char argv[])
{
    struct timespec start, end;
    unsigned long elapsed_seconds = 0;
    unsigned long elapsed_nanoseconds = 0;
    unsigned long int i = 10000;
    unsigned long int j = 0;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    j = i/256;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    elapsed_seconds = end.tv_sec - start.tv_sec;
    elapsed_nanoseconds = end.tv_nsec - start.tv_nsec;
    printf("Time elapsed is %ld seconds and %ld nanoseconds\n", elapsed_seconds, elapsed_nanoseconds);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    j = i >> 8;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    elapsed_seconds = end.tv_sec - start.tv_sec;
    elapsed_nanoseconds = end.tv_nsec - start.tv_nsec;
    printf("Time elapsed is %ld seconds and %ld nanoseconds\n", elapsed_seconds, elapsed_nanoseconds);

    return 0;
}
