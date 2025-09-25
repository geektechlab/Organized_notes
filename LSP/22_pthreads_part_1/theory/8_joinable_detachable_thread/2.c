#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *threadFn(void *arg)
{
 //pthread_detach(pthread_self());
 printf("Thread Fn\n");
 pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
 pthread_t tid;
 int ret = pthread_create(&tid, NULL, threadFn, NULL);

 if (ret != 0) {
  perror("Thread Creation Error\n");
  exit(1);
 }
 printf("After thread created in Main\n");
 sleep(1);
 return 0;
}
