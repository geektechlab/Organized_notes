//Example Code to check whether created thread is running in the same process or different process

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* threadFn(void *arg)
{
 printf("%s:%d\n",__func__, getpid());
 pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

 pthread_t tid;
 int ret = pthread_create(&tid, NULL, threadFn, NULL);
 if (ret != 0){
  perror("Error Creating Thread\n");
  exit(1);
 }
 pthread_join(tid, NULL);
 printf("%s:%d\n",__func__, getpid());
 return 0;
}

