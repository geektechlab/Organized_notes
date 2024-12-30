/*
 * =====================================================================================
 *
 *       Filename:  hello_world.c
 *
 *    Description: This file demonstrates the use of POSIX threads - A hello world program 
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
 * gcc -g -c hello_world.c -o hello_world.o
 * gcc -g hello_world.o -o hello_world.exe -lpthread
 * Run : ./hello_world.exe
 */

/* Notes:
in order to write a multithreaded program in C, you need to hash include this header file, which is pthreads. So it is one headerfile which you need to hash include whenever you need to work with Posix threads.

If if you simply create an executable of this Hello World program at this point of time and if you simply run the program, it will create one thread which is called a main thread and that main thread will execute the function main function. any application has at least one thread and that is called a main thread. So let us start with our development from the main function. from the main function, I will going to create a new thread. So in order to create a new thread, I will go into write a user defined function called Thread one Create. Shortly I will show you the implementation of this function and since it is a main thread which will be the parent thread of the new thread which it will going to create.

Now remember one point that if main thread terminates then all the child thread which main thread has created also get terminated. So we would want the main thread to not to terminate. If your main thread terminates, your entire application is terminated. So whatever child threads your application has created, all those child threads will also be get terminated. So we would not want our main thread to terminate. And therefore I have inserted this pause function right in order to make use of this pause function, you must have to include the header file hash include unistd.h. So as you run this program, your main function will get halt at this pause line and It will not get terminated.

In this function we will going to create a child thread. every thread is identified by a data structure called p thread underscore T. So it's an inbuilt data structure which is defined in the header file p thread dot h. As an end programmer or developer, you don't have to worry about the internal definition of this data structure, meaning that you don't have to worry about what other members of this data structures This data structure is opaque to a programmer or developer. When I say opaque, it simply means that as a developer you never need to know what other internal members of this data structure. it is an opaque data structure which has been exposed to the users or developer to work with threads. So this P thread underscore T is also popularly called as thread handle.

So since we need to create a new thread, we need a thread handle using which we identify that particular thread. In other words, you can think of it as a variable which you can use in your program whenever you need to perform operation on the new thread which is represented by this thread handle. Now, having created this thread handle, let us take some input which would serve as an input to the thread that we were going to create. So you are going to create a new child thread and a thread are supposed to do some work and in order to do some work you need some input. Just like you have functions which do some work and functions need input. In the similar way, the threads also need input.

Now that we have taken an input data which needs to be provided to the child thread that we will going to create. The new thread is created using API P thread underscore create. Now this is an inbuilt API which is provided by our Posix thread library. The first argument to this API is nothing but the thread handle. the fourth argument is nothing but a data or input data which we need to supply as an input to our new thread. So in the fourth argument you need to pass the address of the memory and this memory will be consumed by the new thread as an input. So always remember that the memory which is provided as an input to the new thread must be the memory on the heap or it must be the static memory. It should not be the memory which represents a local variable in the function which is creating a thread. So this chunk of memory should be either static variable or it should be a memory on the heap. When I say a memory on the heap, it simply means that it is dynamically allocated memory. It is very wrong If you pass the input to the thread, the address of a local variable, in other words, the address of a memory which is on the stack.

line number 82 is a fork point. One create function is called and it is being executed in the context of a main thread. It is the main thread which is executing, but at line number 82, the main thread is now giving birth to a child thread. So line number 82 is your fork point. Now you can read these useful comments that p thread underscore create API returns zero on success. Otherwise it returns a negative value which represents an error.

you can always print the value which is returned by this p thread. Underscore create function in order to know that what is the error code returned right from that error code you can find out that if your creation of the thread fails.

why the last argument of the p thread underscore create function has to be a memory which is either on the heap or static memory. The reason is that that at fork point that is line number 82, a new thread will going to be launched. And while the new thread starts executing the function thread, one underscore create would going to be returned. That is, it will complete its execution. And we already know that whenever the function returns, all the local variables inside that functions are destroyed. So it is for this reason that you cannot pass the address of a local variable of the function which contains a fork point. So in this case, if this input was simply a carry star thread input, then this, then passing of this argument would have been very wrong. By the time your new thread starts execution, by the time the new thread starts its execution, the function thread one underscore create had returned and this memory which was the local variable, would have been destroyed. So it means that whatever argument or input you had provided to your new thread is actually an invalid memory. So it is for this reason that this input either should be static memory or it should be a memory on the heap.

third function callback is a function which the new thread will going to execute. Write Such type of functions have a fixed prototype and the prototype should be should be that that these function must return void star and the argument to this function is also void star. as soon as the child thread starts, the child thread invokes this function and this function will be executed in the context of a child thread.

whatever memory you passed as the last argument to the P thread create function, the address of that memory will be available as an argument to this thread function callback. now I will create an infinite loop and inside that loop I just print this string I will use sleep one so that our new thread take a rest of one second.

note that for a multithreaded program you need to link your executable with the P thread library.

while executing, if you just press enter the hello world executable will be created and the expectation is that that the main thread should get paused right here and the child thread must run in this infinite loop. So the child thread is running and it is printing the input string after every one second on the screen. In other words, the child thread is doing its job, whereas the main thread is halt at the line number 106. If we remove this line number 106 and if you execute this program again after compiling, of course, then you will see that nothing happens because Even before your child thread took a birth and start its life, your main thread had terminated and if your main thread terminates, then all the child threads in the program gets terminated automatically. by removing this pause call from the function, we have hardly given any chance to the child thread to take a birth and live its life.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> /* For working with POSIX threads*/
#include <unistd.h>  /* For pause() and sleep() */
#include <errno.h>	 /* For using Global variable errno */

/* A thread callback fn must have following prototypes 
 * void *(*thread_fn)(void *)
 * */
static void *
thread_fn_callback(void *arg) {

	char *input = (char *)arg;

	while(1) {	
		printf("input string = %s\n", input);
		sleep(1);
	}
}

void
thread1_create() {

	/* opaque object, dont bother about its internal
	 * members */
	pthread_t pthread1;

	/* Take some argument to be passed to the thread fn,
 	 * Look after that you always paas the persistent memory
 	 * as an argument to the thread, do not pass caller's 
 	 * local variables Or stack Memory*/	
	static char *thread_input1 = "I am thread no 1";

	/* Return 0 on success, otherwise returns errorcode, all
 	 * pthread functions return -ve error code on failure, they
 	 * do not set global 'errno' variable */
	int rc = pthread_create(&pthread1, 
				   NULL, 
				   thread_fn_callback,
				   (void *)thread_input1);
	if(rc != 0) {

		printf("Error occurred, thread could not be created, errno = %d\n", rc);
		exit(0);
	}
}

int
main(int argc, char **argv){

	printf("main fn paused\n");
	printf("main fn paused\n");
	thread1_create();
	printf("main fn paused\n");
	pause();
	return 0;
}

