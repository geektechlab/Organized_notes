#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

int main(int argc, char argv[])
{
    struct timespec start, end;
    unsigned long elapsed_seconds = 0;
    unsigned long elapsed_nanoseconds = 0;
    unsigned long int i = 0;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    while (i < INT_MAX)
	i++;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    elapsed_seconds = end.tv_sec - start.tv_sec;
    elapsed_nanoseconds = end.tv_nsec - start.tv_nsec;
    printf("Time elapsed is %ld seconds and %ld nanoseconds\n", elapsed_seconds, elapsed_nanoseconds);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    i = 0;
    while (i < UINT_MAX)
	i++;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    elapsed_seconds = end.tv_sec - start.tv_sec;
    elapsed_nanoseconds = end.tv_nsec - start.tv_nsec;
    printf("Time elapsed is %ld seconds and %ld nanoseconds\n", elapsed_seconds, elapsed_nanoseconds);

    return 0;
}
