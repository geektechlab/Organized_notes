#include <stdio.h>
#include <pthread.h>
#include <limits.h>

/* pthread_cond_broadcast will signal all the threads and all can proceed ahead */

pthread_mutex_t my_mutex = PTHREAD_MUTEX_INITIALIZER;        
pthread_cond_t my_cond = PTHREAD_COND_INITIALIZER;  
unsigned int count = 0;
int thread_id[2] = {1, 2};

void *threadFunc(void *arg)
{
	printf("Thread[%d]:In Thread Function\n", *(int *)arg);
	pthread_mutex_lock(&my_mutex);
	printf("Thread[%d]:Acquired Mutex Lock\n", *(int *)arg);
	pthread_cond_wait(&my_cond, &my_mutex);
	printf("Thread[%d]:Signalled by main thread\n", *(int *)arg);
	for (int n = 0; n < INT_MAX/2; n++)
		count++;
	printf("Thread[%d]:Count:%u\n", *(int *)arg, count);
	pthread_mutex_unlock(&my_mutex);
	printf("Thread[%d]:Unlocking Mutex\n", *(int *)arg);
	pthread_exit(NULL);
}

int main()
{
	pthread_t tid[2];

	printf("%s:Creating Thread\n", __func__);
	pthread_create(&tid[0], NULL, threadFunc, &thread_id[0]);
	pthread_create(&tid[1], NULL, threadFunc, &thread_id[1]);
	for (int n = 0; n < INT_MAX/2; n++)
		count++;
	printf("%s:Acquired Mutex Lock\n", __func__);
	pthread_mutex_lock(&my_mutex);
	printf("%s:Count:%u\n", __func__, count);
	printf("%s: signalling thread\n", __func__);
	pthread_cond_broadcast(&my_cond);
	pthread_mutex_unlock(&my_mutex);
	printf("%s:Unlocking Mutex\n", __func__);
	pthread_exit(NULL);
}
