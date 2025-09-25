#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void * thread_func(void *);
void say_hello();

pthread_key_t thr_id_key;

void main(int argc, void ** argv)
{
    pthread_t thread[10];
    int * thread_id;
    int i;

    pthread_key_create(&thr_id_key, NULL);

    for(i = 1; i <= 10; i++)
    {
        thread_id = (int *)malloc(sizeof(int));
        *thread_id = i;
        pthread_create(&thread[i], NULL,
                       thread_func, (void *)(thread_id));
    }

    pthread_exit(NULL);
}

void * thread_func(void * thread_id)
{
    pthread_setspecific(thr_id_key, thread_id);

    say_hello();

    return NULL;
}

void say_hello()
{
    printf("Thread [%d] says Hello!\n",
           *((int *)pthread_getspecific(thr_id_key)));
}
