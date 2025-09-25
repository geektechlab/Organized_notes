#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void * threadfunc(void *arg)
{
	long datum = (long)arg;
	printf("Worker thread #%5ld: pausing now...\n", datum);
	pause();
	printf(" #%5ld: work done, exiting now\n", datum);
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	long i;
	int ret;
	pthread_t tid;
	long numthrds=0;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s number-of-threads-to-create\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	numthrds = atol(argv[1]);
	if (numthrds <= 0) {
		fprintf(stderr, "Usage: %s number-of-threads-to-create\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < numthrds; i++) {
		ret = pthread_create(&tid, NULL, threadfunc, (void *)i);
		if (ret)
			printf("pthread_create() #%d failed! [%d]\n", i, ret);
	}

	pthread_exit(NULL);
}

