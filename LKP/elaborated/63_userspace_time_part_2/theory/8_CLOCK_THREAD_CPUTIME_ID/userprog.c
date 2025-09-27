#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>

void* thread_fn(void *arg)
{
	struct timespec start_threadtime, end_threadtime;
	unsigned int i = 0;
	unsigned long elapsed_seconds = 0;
	unsigned long elapsed_nanoseconds = 0;

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start_threadtime);
	while (i < INT_MAX)
		i++;
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end_threadtime);
	elapsed_seconds = end_threadtime.tv_sec - start_threadtime.tv_sec;
	elapsed_nanoseconds = end_threadtime.tv_nsec - start_threadtime.tv_nsec;
	printf("Thread Time elapsed is %ld seconds and %ld nanoseconds\n", elapsed_seconds, elapsed_nanoseconds);

	return 0;
}

int main(int argc, char argv[])
{
    struct timespec start_cputime, end_cputime, start_threadtime, end_threadtime;
    struct timespec start_realtime, end_realtime;
    unsigned long elapsed_seconds = 0;
    unsigned long elapsed_nanoseconds = 0;
    unsigned int i = 0;
    pthread_t thread;

    clock_gettime(CLOCK_REALTIME, &start_realtime);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_cputime);
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start_threadtime);

    pthread_create(&thread, NULL, thread_fn, NULL);
    while (i < UINT_MAX)
	    i++;

    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end_threadtime);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_cputime);
    clock_gettime(CLOCK_REALTIME, &end_realtime);

    elapsed_seconds = end_threadtime.tv_sec - start_threadtime.tv_sec;
    elapsed_nanoseconds = end_threadtime.tv_nsec - start_threadtime.tv_nsec;
    printf("Main Thread Time elapsed is %ld seconds and %ld nanoseconds\n", elapsed_seconds, elapsed_nanoseconds);

    elapsed_seconds = end_realtime.tv_sec - start_realtime.tv_sec;
    elapsed_nanoseconds = end_realtime.tv_nsec - start_realtime.tv_nsec;
    printf("Real Time elapsed is %ld seconds and %ld nanoseconds\n", elapsed_seconds, elapsed_nanoseconds);

    elapsed_seconds = end_cputime.tv_sec - start_cputime.tv_sec;
    elapsed_nanoseconds = end_cputime.tv_nsec - start_cputime.tv_nsec;
    printf("CPU Time elapsed is %ld seconds and %ld nanoseconds\n", elapsed_seconds, elapsed_nanoseconds);

    return 0;
}
