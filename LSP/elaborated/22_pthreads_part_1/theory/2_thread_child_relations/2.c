#include <stdio.h>
#include <pthread.h>

void *func(void *arg) {
    printf("%s\n", (char *)arg);
    return NULL;
}

int main() {
    pthread_t id;
    pthread_create(&id, NULL, func, "Linux World");
    while (1) {} // Loop forever
}
