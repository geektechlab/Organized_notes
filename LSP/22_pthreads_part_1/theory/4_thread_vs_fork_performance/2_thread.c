#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMTHREADS 50000

void *thread_func(void *arg)
{
	unsigned long t = 0;
	t++;
	pthread_exit(NULL);
}

int main(void)
{
	int ret, i;
	pthread_t tid;


	for (i = 0; i < NUMTHREADS; i++) {
		ret = pthread_create(&tid, NULL, thread_func, NULL);
		if (ret)
			printf("pthread_create() failed! [%d]\n", ret);

		/* Wait for the thread to terminate */
		ret = pthread_join(tid, NULL);
		if (ret)
			printf("pthread_join() failed! [%d]\n", ret);
	}
	pthread_exit(NULL);
}

