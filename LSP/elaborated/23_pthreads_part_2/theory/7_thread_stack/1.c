#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

/* check address of each local variables' of stack in each thread */

void *func(void *arg) {
    int local_var = 10;
    printf("Thread ID:%lu \t local variable address:%p\n", pthread_self(), &local_var);
    pthread_exit(NULL);
}

int main() {
   
    pthread_t id;
    int local_var = 10;
    printf("PID:%d\n", getpid());
    getchar();
    printf("Thread ID:%lu \t local variable address:%p\n", pthread_self(), &local_var);
    pthread_create(&id, NULL, func, NULL);
    pthread_join(id, NULL);
}
