#include <stdio.h>
#include <pthread.h>
#include <limits.h>

pthread_mutex_t my_mutex = PTHREAD_MUTEX_INITIALIZER;        
pthread_cond_t my_cond = PTHREAD_COND_INITIALIZER;  
int count = 0;

void *threadFunc(void *arg)
{
	printf("In Thread Function\n");
	pthread_mutex_lock(&my_mutex);
	printf("%s:Acquired Mutex Lock\n", __func__);
	pthread_cond_wait(&my_cond, &my_mutex);
	printf("%s:Signalled by main thread\n", __func__);
	for (int n = 0; n < INT_MAX/2; n++)
		count++;
	printf("%s:Count:%d\n", __func__, count);
	pthread_mutex_unlock(&my_mutex);
	printf("%s:Unlocking Mutex\n", __func__);
	pthread_exit(NULL);
}

int main()
{
	pthread_t tid;

	printf("%s:Creating Thread\n", __func__);
	pthread_create(&tid, NULL, threadFunc, NULL);
	for (int n = 0; n < INT_MAX/2; n++)
		count++;
	printf("%s:Acquired Mutex Lock\n", __func__);
	pthread_mutex_lock(&my_mutex);
	printf("%s:Count:%d\n", __func__, count);
	printf("%s: signalling thread\n", __func__);
	pthread_cancel(tid);
	pthread_cond_signal(&my_cond);
	pthread_mutex_unlock(&my_mutex);
	printf("%s:Unlocking Mutex\n", __func__);
	pthread_exit(NULL);
}
