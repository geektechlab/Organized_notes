//How can I create ten threads with different starting values.
#include <stdio.h>
#include <pthread.h>

void* func(void* arg) 
{
    int i = *((int *) arg);
    printf("%d\n", i);
    pthread_exit(NULL);
}

int main() {
    // Each thread gets a different value of i to process
    int i;
    pthread_t tid;
    for(i =0; i < 10; i++) {
        pthread_create(&tid, NULL, func, &i); 
	//pthread_join(tid, NULL);
    }
    pthread_exit(NULL);
}

