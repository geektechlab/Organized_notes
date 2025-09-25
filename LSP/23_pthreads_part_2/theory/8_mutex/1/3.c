#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* if we forget to pthread_mutex_unlock then it will freeze ? */

pthread_mutex_t *mtx;
int glob = 0;

void *threadFunc(void *arg)
{
	int j;
	int loops = *((int *) arg);
	printf("\n %s: \n",__func__);
	for (j = 0; j < loops; j++)
	{
		 pthread_mutex_lock(mtx);
		 glob++;
		// pthread_mutex_unlock(mtx);
	}

	return NULL;
}

int main()
{
	pthread_t t1, t2;
	int loops=1000000, ret;

	mtx = malloc(sizeof(pthread_mutex_t));
	// Initilize the mutex and its underlying resources
	pthread_mutex_init(mtx, NULL);

	ret = pthread_create(&t1, NULL, threadFunc, &loops);
	if (ret != 0)
		perror("Pthread Create :   ");

	ret = pthread_create(&t2, NULL, threadFunc, &loops);
	if (ret != 0)
		perror("Pthread Create:    ");

	ret = pthread_join(t1, NULL);
	if (ret != 0)
		perror("Pthread Join:     ");

	ret = pthread_join(t2, NULL);
	if (ret != 0)
		perror("Pthread Join :    ");

	printf("glob = %d\n", glob);
	pthread_mutex_destroy(mtx);

	return 0;
}

