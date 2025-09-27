#include <pthread.h>
#include <stdio.h>

/* terminating main thread doesn't terminate "child" threads */

void *print(void *arg)
{
	int *i = (int *) arg;
	printf("Hi.  I'm thread %d\n", *i);
	pthread_exit(NULL);
}

int main()
{
	int i;
	pthread_t tids[4], value[4];
	void *retval;

	for (i = 0; i < 4; i++) {
		value[i] = i;
		pthread_create(tids+i, NULL, print, (value+i));
	}

	pthread_exit(NULL);
	for (i = 0; i < 4; i++) {
		printf("Trying to join with tid %d\n", i);
		pthread_join(tids[i], &retval);
		printf("Joined with tid %d\n", i);
	}
	return 0;
}
