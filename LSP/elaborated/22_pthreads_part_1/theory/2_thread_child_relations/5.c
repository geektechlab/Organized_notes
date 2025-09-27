#include <stdio.h>
#include <string.h>
#include <pthread.h>

void *thread_fn(void *arg)
{
 char *str = (char *)arg;
 printf("Thread Fn:%s\n", str);
 return (void *)strlen(str);
}

int main(int argc, char *argv[])
{
 pthread_t tid;
 void *res;
 int ret;

 ret = pthread_create(&tid, NULL, thread_fn, "Hello World");
 if (ret != 0)
  printf("Error in creating Thread\n");

 pthread_join(tid, &res);
 printf("Thread returned :%ld\n", (long)res);
 
 return 0;
}
