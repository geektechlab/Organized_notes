/*
 * =====================================================================================
 *
 *       Filename:  joinable_threads.c
 *
 *    Description: This file demonstrates the use of POSIX threads joinable API 
 *
 *        Version:  1.0
 *        Created:  11/03/2020 07:50:04 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ABHISHEK SAGAR (), sachinites@gmail.com
 *   Organization:  Juniper Networks
 *
 * =====================================================================================
 */

/*
 * compile using :
 * gcc -g -c joinable_threads.c -o joinable_threads.o
 * gcc -g joinable_threads.o -o joinable_threads.exe -lpthread
 * Run : ./joinable_threads.exe
 */

/* these are the two thread handle for the two child threads which are main thread will going to fork. Now the functionality of this program is that that when our main thread creates child threads, then at the time of creation of child threads, our main thread will supply an integer value as an argument to these child threads. These child threads simply compute the square of that integer value and return the result to the main thread.

let us discuss the implementation of the function thread underscore create. thread underscore create function accepts two argument. The first argument is a pointer to the thread handle and the second argument is the integer value that we have passed. Let us call that integer value as thread id. Now, the responsibility of this function is to create a new thread.

The Posix standard provides us the thread attribute. So in this API, simply pass the address of the attribute variable and pass whether and pass the flag which states whether your thread is a joinable thread or detached thread. Suppose Posix standard provides this constant value which is pthreadcreate joinable. If you want to create a detached thread, then in that case you should have specified the value pthreadcreate detached. So depending what you pass as the second parameter to this attribute, the new thread that you are going to create will be joinable or detached.

now we are in a position to invoke an API thread, underscore, create in order to launch a new thread. line number 88 is a fork point and let us create a thread pass the thread handle for which you are creating a thread. Pass the attribute which we have specified as Joinable. Pass the thread callback function which we will going to specify shortly and pass the argument to the function. Now the argument to the function is nothing, but it is a integer number which we have passed as a second argument to thread create function. And as I told you that we should always pass the heap memory storage or static storage as an argument to the function. So it is for this reason that I will going to take a heap memory to store this integer input. So here I am, passing the address of the heap memory and not the local variable.

So line number 44 is a fork point and it will going to create a thread which will start execute in the function thread function callback. Right. Now let us discuss the implementation of this thread callback function. So in the thread function callback, as I said that the only meaningful work that the child thread will going to do is to compute the square of the input which we have passed as an argument to the thread. now and since we have read the value of this argument into a local variable, there is absolutely no need of this argument anymore. We can free this memory.

now coming to the main function, you can see that we have now created two threads. The first thread will spend two seconds while doing its work, while the second thread will spend 10s while doing its work right after two seconds the thread two will going to terminate and after 10s the thread three will going to terminate.

So now let us see how to ensure that our main thread which is a parent thread of these two child thread, will going to join these two threads. So now let us make our main thread to wait for the thread number two to join it. It simply means that our main thread has to invoke an API thread, underscore join and it has to pass the handle of the thread. Number two that is p thread two.

now in order to collect the result from the thread Number two, the second argument of the P thread function is used to collect the result which is returned from the child thread. So thread result two is actually a variable of type void star. you have to pass the address of the void star memory location so that when the thread number two will come and join this join point which is on line number 88, then the result of the child thread will be pointed by this pointer white thread result to.

P thread join is a blocking API remember and main thread will stay blocked until the child thread p thread to join our main thread. So when our main thread will get unblocked at p thread join function. The result returned by the p thread two will be available in this memory location.

after collecting the result from the child thread, never forget to release the memory. It is the responsibility of the parent thread to release the memory which is used by the child thread to provide results.

So now in this program, if I ask you one question, what would have been the scenario if P thread number two was made to run for 10s and then P thread number three was made to run for two seconds, right. So I would request you to change these values and try to understand by observing how your program behaves when you made the child threads to wait for different different amount of time intervals. */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> /* For working with POSIX threads*/
#include <unistd.h>  /* For pause() and sleep() */
#include <errno.h>	 /* For using Global variable errno */

/* A thread callback fn must have following prototypes 
 * void *(*thread_fn)(void *)
 * */
static void * thread_fn_callback(void *arg)
{
	int th_id = *(int *)arg ;
	int rc = 0 ;

	free(arg);

	while(rc != th_id) {

		printf("Thread %d doing some work\n", th_id);
		sleep(1);
		rc++;
	}

	int *result = calloc(1, sizeof(int));
	*result = th_id * th_id;
	return (void *)result;
}


void
thread_create(pthread_t *pthread_handle, int th_id) {

	pthread_attr_t attr;
	
	int *_th_id = calloc(1, sizeof(int));
	*_th_id = th_id;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE /* PTHREAD_CREATE_DETACHED  */);
	
	int rc = pthread_create(pthread_handle, 
				   &attr,
				   thread_fn_callback,
				   (void *)_th_id);
	if(rc != 0) {

		printf("Error occurred, thread could not be created, errno = %d\n", rc);
		exit(0);
	}
}

pthread_t pthread2;
pthread_t pthread3;

int
main(int argc, char **argv){

	void *thread_result2 = NULL;
	void *thread_result3 = NULL;

	thread_create(&pthread2, 2);
	
	thread_create(&pthread3, 10);

	printf("main fn blocked on pthread join for thread with th_id = 2\n");
	pthread_join(pthread2, &thread_result2);
	if(thread_result2) {
		printf("Return result from thread 2 = %d\n", *(int *)thread_result2);
		free(thread_result2);
		thread_result2 = NULL;
	}	

	printf("main fn blocked on pthread join for thread with th_id = 3\n");
	pthread_join(pthread3, &thread_result3);
	if(thread_result3) {
		printf("Return result from thread 3 = %d\n", *(int *)thread_result3);
		free(thread_result3);
		thread_result3 = NULL;
	}	

	return 0;
}

