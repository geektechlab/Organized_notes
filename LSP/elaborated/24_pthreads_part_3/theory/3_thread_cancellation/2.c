#include <stdio.h>
#include <pthread.h>
#include <limits.h>

void *threadFunc(void *arg)
{
	int state,type;
	if(pthread_setcancelstate(NULL,&state)==0)
	{
		switch(state)
		{
			case PTHREAD_CANCEL_ENABLE:
				printf("Cancel State Enable\n");
				break;
			case PTHREAD_CANCEL_DISABLE:
				printf("Cancel State Disable\n");
		}
	}

	if(pthread_setcanceltype(NULL,&type)==0)
	{
		switch(type)
		{
			case PTHREAD_CANCEL_DEFERRED:
				printf("\n Cancel Type Deferred\n");
				break;
			case PTHREAD_CANCEL_ASYNCHRONOUS:
				printf("\n Cancel Type Asynchoronous\n");
		}
	}
	pthread_exit(NULL);
}

int main()
{
	pthread_t tid;

	printf("%s:Creating Thread\n", __func__);
	pthread_create(&tid, NULL, threadFunc, NULL);
	pthread_exit(NULL);
}
