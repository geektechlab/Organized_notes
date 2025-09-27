#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

int main(int argc, char argv[])
{
    struct timespec start_cputime, end_cputime, start_monotonic, end_monotonic;
    unsigned long elapsed_seconds = 0;
    unsigned long elapsed_nanoseconds = 0;
    int val = 0;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_cputime);
    clock_gettime(CLOCK_MONOTONIC, &start_monotonic);
    scanf("%d", &val);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_cputime);
    clock_gettime(CLOCK_MONOTONIC, &end_monotonic);

    elapsed_seconds = end_cputime.tv_sec - start_cputime.tv_sec;
    elapsed_nanoseconds = end_cputime.tv_nsec - start_cputime.tv_nsec;
    printf("CPU Time elapsed is %ld seconds and %ld nanoseconds\n", elapsed_seconds, elapsed_nanoseconds);

    elapsed_seconds = end_monotonic.tv_sec - start_monotonic.tv_sec;
    elapsed_nanoseconds = end_monotonic.tv_nsec - start_monotonic.tv_nsec;
    printf("Monotonic Time elapsed is %ld seconds and %ld nanoseconds\n", elapsed_seconds, elapsed_nanoseconds);
    return 0;
}


