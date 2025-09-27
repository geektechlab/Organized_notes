#include <stdio.h>
#include <pthread.h>
#include <limits.h>

int count;

void *threadFunc(void *arg)
{
	int state,type;
	printf("In Thread Function\n");
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	printf("%s:Set Cancel State to disabled\n", __func__);
	for (int n = 0; n < INT_MAX; n++)
		count++;
	printf("%s: Count:%d\n", __func__, count);
	printf("Thread Exits\n");
	pthread_exit(NULL);
}

int main()
{
	pthread_t tid;

	printf("%s:Creating Thread\n", __func__);
	pthread_create(&tid, NULL, threadFunc, NULL);
	sleep(1);
	printf("%s:Sending cancellation request to thread and exit\n", __func__);
	pthread_cancel(tid);
	pthread_exit(NULL);
}
