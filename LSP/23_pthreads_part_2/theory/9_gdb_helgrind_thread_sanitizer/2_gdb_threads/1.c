#include <stdio.h>
#include <pthread.h>

pthread_t tid[2];

void *thread_fn(void *arg)
{
	if(pthread_equal(tid[0], pthread_self()))
		printf("Thread 0 in execution\n");
	else
		printf("Thread 1 in execution\n");
	pthread_exit(NULL);
}

int main()
{
	int i;
	int ret;
	for (i = 0;i < 2;i++) {
		ret = pthread_create(&tid[i], NULL, thread_fn, NULL);
		if (!ret)
			printf("Thread[%d] created Successfully\n",i+1);
		else
			printf("Thread[%d] not created\n",i+1);
	}

	pthread_exit(NULL);
	printf("After thread execution completed\n");

	return 0;
}
