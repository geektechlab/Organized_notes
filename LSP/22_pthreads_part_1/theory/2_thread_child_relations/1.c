#include <stdio.h>
#include <pthread.h>
#include <sys/syscall.h>

/* command to compile: gcc 1.c -o 1 -lpthread */

void *func(void *arg) {
    printf("Hello World\n");
    printf("Process ID:%u\t Thread ID:%lu\n",
                        getpid(), syscall(SYS_gettid));

    return NULL;
}

int main() {
    pthread_t id;
    printf("Process ID:%u\t Thread ID:%lu\n",
                        getpid(), syscall(SYS_gettid));
    pthread_create(&id, NULL, func, NULL);
    while (1) {} // Loop forever
}
