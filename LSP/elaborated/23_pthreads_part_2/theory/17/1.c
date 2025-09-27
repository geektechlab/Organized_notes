#include<stdio.h>
#include<pthread.h>
pthread_rwlock_t rwlock_id;
int var_global  = 4;

void * Read_Lock_Thread()
{
    int ret, i=0;
	while(i <2 )
	{
		ret = pthread_rwlock_rdlock(&rwlock_id);
		if(ret != 0)
			perror("error in read lock taking");
		printf("READ : value of global variable is %d\n", var_global);
		sleep(10);
		ret = pthread_rwlock_unlock(&rwlock_id);
		if(ret != 0)
			perror("read un-lock");
		i++;	
		sleep(1);
	}
}

void * Write_Lock_Thread()
{
	int ret;
	ret = pthread_rwlock_wrlock(&rwlock_id);
	if(ret != 0)
		perror("error in write lock taking");
	printf(" WRITE : value of global variable is %d\n", var_global);
	var_global = 5;
	printf(" WRITE : value of global variable changed is %d\n", var_global);
	ret = pthread_rwlock_unlock(&rwlock_id);
	if(ret != 0)
		perror("Write un-lock");
}

int main()
{
	int ret;
	pthread_t thread_id[2];
	// initializing read write lock
	ret = pthread_rwlock_init(&rwlock_id, NULL);
	if(ret != 0)
		perror("RWlock initialize ");
	// Creates thread which only read the value
	ret = pthread_create(&thread_id[0], NULL, Read_Lock_Thread, NULL);
	if(ret != 0)
		perror("Read lock thread ");
	// Creates thread which only Write the value
	ret = pthread_create(&thread_id[1], NULL, Write_Lock_Thread, NULL);
	if(ret != 0)
		perror("Write lock thread ");
	sleep(2);
	ret = pthread_rwlock_rdlock(&rwlock_id);
	if(ret != 0)
		perror("error in read lock taking");
	printf(" MAIN : value of global variable is %d\n", var_global);
	ret = pthread_rwlock_unlock(&rwlock_id);
	if(ret != 0)
		perror("Write un-lock");
	
	pthread_join(thread_id[0],NULL);	
	pthread_join(thread_id[1],NULL);	
}

