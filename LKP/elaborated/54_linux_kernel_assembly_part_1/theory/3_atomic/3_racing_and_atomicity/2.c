#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* here, 2 threads are operating on glob global variable, so race condition can happen. Also, increment is not atomic if we think in assembly perspective, then each other can preempt other and racing can happen */
/* incl instruction is also unable to help here, maybe be because of other processor accessing it */

int glob = 0;

void *threadFunc(void *arg)
{
	int j;
	int loops = *((int *) arg);
	printf("\n %s: start\n",__func__);
	for (j = 0; j < loops; j++)
	{
		asm volatile("incl %0"
			     :"+m"(glob)
			     :
			     : "memory");
	}
	printf("\n %s: end\n",__func__);

	return NULL;
}

int main()
{
	pthread_t t1, t2;
	int loops=1000000, ret;

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
	return 0;
}
