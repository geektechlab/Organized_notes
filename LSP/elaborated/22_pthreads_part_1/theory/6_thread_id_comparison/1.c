#include <stdio.h>
#include <pthread.h>

/* print thread ids */

void *thread_fn(void *arg)
{
 sleep(1);
 printf("ID of thread in thread_fn is %u\n",pthread_self());
 return NULL;
}

int main(int argc, char *argv[])
{
 pthread_t tid;
 printf("In Main Fn\n");
 pthread_create(&tid, NULL, thread_fn, NULL);
 printf("ID of thread in main:%u\n",tid);
 pthread_join(tid, NULL);
 printf("After the thread execution\n");
 return 0;
}
