#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* deadlock */

pthread_mutex_t *mtx1, *mtx2;

void *threadFunc1(void *arg)
{
	printf("\n %s Locking Mutex1: \n",__func__);
	pthread_mutex_lock(mtx1);
	printf("\n %s Mutex1 locked\n", __func__);
	sleep(2);
	pthread_yield();
	printf("\n %s Locking Mutex2: \n",__func__);
	pthread_mutex_lock(mtx2);
	printf("\n %s Mutex2 locked\n", __func__);
	pthread_mutex_unlock(mtx2);
	printf("\n %s Mutex2 unlocked\n", __func__);
	pthread_mutex_unlock(mtx1);
	printf("\n %s Mutex1 unlocked\n", __func__);
	pthread_exit(NULL);
}

void *threadFunc2(void *arg)
{
	sleep(1);
	printf("\n %s Locking Mutex2: \n",__func__);
	pthread_mutex_lock(mtx2);
	printf("\n %s Mutex2 locked\n", __func__);
	sleep(2);
	pthread_yield();
	printf("\n %s Locking Mutex1: \n",__func__);
	pthread_mutex_lock(mtx1);
	printf("\n %s Mutex1 locked\n", __func__);
	pthread_mutex_unlock(mtx1);
	printf("\n %s Mutex1 unlocked\n", __func__);
	pthread_mutex_unlock(mtx2);
	printf("\n %s Mutex2 unlocked\n", __func__);
	pthread_exit(NULL);
}

int main()
{
	pthread_t t1, t2;
	int loops=1000000, ret;

	mtx1 = malloc(sizeof(pthread_mutex_t));
	mtx2 = malloc(sizeof(pthread_mutex_t));
	// Initilize the mutex and its underlying resources
	pthread_mutex_init(mtx1, NULL);
	pthread_mutex_init(mtx2, NULL);

	ret = pthread_create(&t1, NULL, threadFunc1, &loops);
	if (ret != 0)
		perror("Pthread Create :   ");

	ret = pthread_create(&t2, NULL, threadFunc2, &loops);
	if (ret != 0)
		perror("Pthread Create:    ");

	ret = pthread_join(t1, NULL);
	if (ret != 0)
		perror("Pthread Join:     ");

	ret = pthread_join(t2, NULL);
	if (ret != 0)
		perror("Pthread Join :    ");

	pthread_mutex_destroy(mtx1);
	pthread_mutex_destroy(mtx2);

	return 0;
}

