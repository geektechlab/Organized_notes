#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_t tid, tid1;

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
	int ret;

	if (pthread_equal(pthread_self(), tid))
		printf("\nPrinting Thread 1 Contents\n");
	else{
		ret = pthread_join(tid, NULL);
		if (ret != 0) {
			perror("Error in joining thread\n");
			exit(1);
		}
		printf("\nPrinting Thread 2 Contents\n");
	}
	attr_dump(pthread_self());
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_attr_t attr;
	int ret, schedpolicy;

	/*
	 * Initializes the atttribute object with the default values.
	 */
	ret = pthread_attr_init(&attr);
	if (ret != 0) {
		perror("Error Initializing attributes\n");
		exit(1);
	}

	ret = pthread_create(&tid, &attr, threadFn, NULL);
	if (ret != 0) {
		perror("Error creating thread\n");
		exit(1);
	}

	ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (ret != 0) {
		perror("Error setting state\n");
		exit(1);
	}

	ret = pthread_attr_setschedpolicy(&attr, SCHED_RR);
	if (ret != 0) {
		perror("Error setting scheduling policy\n");
		exit(1);
	}

	ret  = pthread_create(&tid1, &attr, threadFn, NULL);
	if (ret != 0) {
		perror("Error creating thread\n");
		exit(1);
	}

	/*
	 * When a thread attributes object is no longer, it should be destroyed using
	 * the pthread_attr_destroy() function. Destroying a thread attributes object
	 * has no effect on threads that were created using that object.
	 */
	if (pthread_attr_destroy(&attr) != 0)
		perror("Error destroying attribute\n");
	pthread_exit(NULL); 
}

