#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

/* here, stack overflow will happen because fun is calling itself unconditionally */

void fun()
{
	char array[2048] = {0x01, 0x02};
	printf("hello\n");
	fun();
}

void *func(void *arg) {
    int local_var = 10;
    printf("Thread ID:%lu \t local variable address:%p\n", pthread_self(), &local_var);
    fun();
    pthread_exit(NULL);
}

int main() {
    pthread_t id;
    int local_var = 10;
    printf("PID:%d\n", getpid());
    printf("Thread ID:%lu \t local variable address:%p\n", pthread_self(), &local_var);
    pthread_create(&id, NULL, func, NULL);
    pthread_join(id, NULL);
}
