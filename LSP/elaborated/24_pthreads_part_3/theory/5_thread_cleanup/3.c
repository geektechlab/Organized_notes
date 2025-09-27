#include <pthread.h>
#include <stdio.h>
#include <limits.h>
int count = 0;

void cleanup(void *arg)
{
    printf("cleanup: %s\n", (char *)arg);
}

void *thread_fn(void *arg)
{
    printf("thread 1 start\n");
    pthread_cleanup_push(cleanup, "thread 1 first handler");
    pthread_cleanup_push(cleanup, "thread 1 second handler");
    for (int n = 0; n < INT_MAX/2; n++)
		count++;

    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    printf("thread 1 push complete\n");
}

int main(void)
{
    pthread_t   tid;

   pthread_create(&tid, NULL, thread_fn, NULL);
   sleep(1);
   pthread_cancel(tid);
   pthread_exit(0);
}
