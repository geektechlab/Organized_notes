#include <pthread.h>
#include <stdio.h>

/* creating four threads and joining */

void *print(void *arg)
{
	int *i = (int *) arg;
	printf("Hi.  I'm thread %d\n", *i);
	pthread_exit(NULL);
}

int main()
{
	int i;
	pthread_t tids[4];
	int value[4];
	void *retval;

	for (i = 0; i < 4; i++) {
		value[i] = i;
		pthread_create(tids+i, NULL, print, (value+i));
	}

	for (i = 0; i < 4; i++) {
		printf("Trying to join with tid %d\n", i);
		pthread_join(tids[i], &retval);
		printf("Joined with tid %d\n", i);
	}
	return 0;
}
