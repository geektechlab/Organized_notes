#include <stdio.h> 
#include <signal.h> 
#include <stdlib.h>
#include <unistd.h>

static void rec(int signo) 
{ 
	printf("\ninterrupt received\n"); 
} 

//Suppose my scenario is, I have a program with two critical regions. By critical regions, I mean to say that I don't want some signal to interrupt in between, while my critical section is executing.
//But, at the same time I want my second critical section to start, only if I get the confirmation, that some signal has been received.
//
int main() 
{ 
	sigset_t mask1,mask2;

	signal(SIGINT, rec);
       	sigemptyset(&mask1); 
	sigaddset(&mask1,SIGINT); 
	sigprocmask(SIG_BLOCK,&mask1,&mask2); 
	
	printf("\ncritical region 1\n"); 
	sleep(5); 
	printf("\ncritical region 1 ends\n"); 
	
	sigprocmask(SIG_SETMASK,&mask2,NULL); 
	
	pause(); 
	
	sigprocmask(SIG_BLOCK,&mask1,&mask2); 
	printf("\ncritical region 2\n"); 
	sleep(5); 
	printf("\ncritical region 2 ends\n"); 
	sigprocmask(SIG_SETMASK,&mask2,NULL); 
	exit(0); 

}
