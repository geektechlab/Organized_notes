#include <pthread.h>
#include <stdio.h>

/* return 0 will cause process to get terminated and all threads will end */

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
	return 0;
}
