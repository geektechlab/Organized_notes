#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void attr_dump(pthread_t thread_id)
{
	pthread_attr_t attr;
	void *stkaddr;
	size_t stksize, guardsize;
	int thread_state, schedpolicy;
	/*
	 * pthread_getattr_np : gets the attributes of the thread specified
	 *     in the first argument
	 */
	pthread_getattr_np(thread_id, &attr);
	printf("Printing Default Attributes\n");
	/*
	 * Read stack size and stack address
	 */
	pthread_attr_getstack(&attr, &stkaddr, &stksize);
	printf("Stack Size:%d bytes\n",stksize);
	printf("Stack Address:%p\n",stkaddr);

	/*
	 * Read whether it is detached state or joinable state
	 */
	pthread_attr_getdetachstate(&attr, &thread_state);
	if (thread_state == PTHREAD_CREATE_JOINABLE)
		printf("Thread state:Joinable\n");
	else
		printf("Thread state:Detached\n");

	/*
	 * Read stack guard size
	 */
	pthread_attr_getguardsize(&attr, &guardsize);
	printf("Default stack guard size:%d\n", guardsize);

	/*
	 * Read Scheduling Policy
	 */
	pthread_attr_getschedpolicy(&attr, &schedpolicy);

	if (schedpolicy == SCHED_OTHER)
		printf("Scheduling Policy:SCHED_OTHER(Preemptive fair-share sscheduler)\n");
	else if (schedpolicy == SCHED_RR)
		printf("Scheduling Policy:SCHED_RR(Static Priority Round Robin)\n");
	else if (schedpolicy == SCHED_FIFO)
		printf("Scheduling Policy:SCHED_FIFO(static Priority Preemptive Cooperative scheduler)\n");
}

void *threadFn(void *arg)
{
	pthread_detach(pthread_self());
	attr_dump(pthread_self());
}

int main(int argc, char *argv[])
{
	pthread_t tid;
	int ret;

	ret = pthread_create(&tid, NULL, threadFn, NULL);
	if (ret != 0){
		printf("Error in creating thread\n");
		exit(1);
	}else
		printf("Thread Created Successfully\n");
	pthread_exit(NULL);
}
