#include <stdio.h>
#include <pthread.h>

void *func(void *arg) {
    printf("Hello World\n");
}

int main() {
    pthread_t id;
    pthread_create(&id, NULL, func, NULL);
    sleep(1);
    pthread_join(id, NULL);
    return 0;
}
