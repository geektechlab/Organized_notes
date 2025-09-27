#include <pthread.h>
#include <stdio.h>

int a[45] ;
pthread_barrier_t barrier;

void * thread1(void *arg)
{
	int i ,sum = 0,rc;
	for(i=0;i<15;i++)
		sum+=a[i];
	printf("\n Thread1 waits at barrier\n");
	rc =  pthread_barrier_wait(&barrier);
	printf("\n Thread1 resume\n");
	printf("\n 1 - %d \n",sum);
	printf("\n Thread1 terminates\n");
	*(int *)arg = sum;
	pthread_exit(NULL);
}

void * thread2(void *arg)
{
	int i ,sum = 0;
        for(i=15;i<30;i++)
	{
                sum+=a[i];
	}
	printf("\n Thread2 waits at barrier\n");
		sleep(1);
         pthread_barrier_wait(&barrier);
	printf("\n Thread2 resume\n");
	printf("\n 2 - %d \n",sum);
	printf("\n Thread2 terminates\n");
	*(int *)arg = sum;
	pthread_exit(NULL);

}

void * thread3(void *arg)
{
	int i ,sum = 0;
        for(i=30;i<45;i++)
	{
		sum+=a[i];
	}
	printf("\n Thread3 waits at barrier\n");
         	sleep(2);
         pthread_barrier_wait(&barrier);
	printf("\n Thread3 resume\n");
	printf("\n 3 - %d \n",sum);
	printf("\n Thread3 terminates\n");
	*(int *)arg = sum;
	pthread_exit(NULL);

}

int main()
{
	pthread_t tid[3];
	int i;
	int total_sum = 0;
	int sum;

	for (i = 0 ; i < 45; i++) {
		a[i] = i+1;
	}
	if(pthread_barrier_init(&barrier, NULL, 3))
	{
        	printf("Could not create a barrier\n");
        	return -1;
    }
	pthread_create(&tid[0], NULL, thread1, &sum);
	pthread_create(&tid[1], NULL, thread2, &sum);
	pthread_create(&tid[2], NULL, thread3, &sum);
	pthread_join(tid[0],NULL);
	total_sum += sum;
	pthread_join(tid[1],NULL);
	total_sum += sum;
	pthread_join(tid[2],NULL);
	total_sum += sum;
	printf("Total sum:%d\n", total_sum);
	pthread_exit(NULL);
}
