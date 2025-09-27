#include <stdio.h>
#include <pthread.h>

void *ThreadFn(void *arg)
{
 sleep(1);
 printf("In Thread Fn\n");
 return NULL;
}

int main(int argc, char *argv[])
{
 
 pthread_t tid;
 printf("Before Thread\n");
 pthread_create(&tid, NULL, ThreadFn, NULL);
 /*
   *pthread_join blocks the calling thread until the
  * thread with identifier equals to the first argument
  * terminates
   */
 pthread_join(tid, NULL);
 printf("After Thread\n");
 return 0;
}
