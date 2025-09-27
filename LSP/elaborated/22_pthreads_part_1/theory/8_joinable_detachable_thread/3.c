#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

/* other way to detach thread */

void *threadFn(void *arg)
{
 printf("Thread Fn\n");
 pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  pthread_attr_t attr;

 pthread_t tid;
 int ret;

  /*
  * Initializes the atttribute object with the default values.
  */
 ret = pthread_attr_init(&attr);

 if (ret != 0) {
  perror("Error Initializing attributes\n");
  exit(1);
 }

  ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

 if (ret != 0) {
  perror("Error setting state\n");
  exit(1);
 }

 ret = pthread_create(&tid, &attr, threadFn, NULL);

 if (ret != 0) {
  perror("Thread Creation Error\n");
  exit(1);
 }

 printf("After thread created in Main\n");
 sleep(1);
 return 0;
}
