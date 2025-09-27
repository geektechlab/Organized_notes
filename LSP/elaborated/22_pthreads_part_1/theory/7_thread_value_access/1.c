#include <stdio.h>
#include <pthread.h>

/* it is not guarnteed that func1 will be able to access 20 val because fucn can get scheduled and terminated well before func1 get scheduled */

void *func1(void *arg)
{
	printf("val:%d\n", *(int *)arg);
}

void *func(void *arg) {
    int val = 20;
    pthread_t tid;
    printf("Hello World\n");
    pthread_create(&tid, NULL, func1, &val);
    pthread_exit(NULL);
}

int main() {
    pthread_t id;
    pthread_create(&id, NULL, func, NULL);
    pthread_exit(NULL);
}
