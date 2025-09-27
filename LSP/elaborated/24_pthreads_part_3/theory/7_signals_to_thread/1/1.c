#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h> /*for signal() and raise()*/
#include <limits.h>

/* only once signal handler will get exected */

int count = 0;

void *threadfunc(void *arg)
{
	printf("In thread function\n");
	for (int i = 0 ; i < INT_MAX; i++)
		count++;

	printf("Completed thread function\n");
	pthread_exit(NULL);
}

void hello(int signum){
  printf("Hello World!\n");
}

int main(){

  pthread_t tid;
  //execute hello() when receiving signal SIGUSR1  
  signal(SIGUSR1, hello);

  pthread_create(&tid, NULL, threadfunc, NULL);
  sleep(1);
  //send SIGUSR1 to the calling process  
  raise(SIGUSR1);
  pthread_exit(NULL);
}
