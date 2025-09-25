#include <stdio.h>
#include <pthread.h>

void *func(void *arg)
{
 return NULL;
}

int main(int argc, char *argv[])
{
 pthread_t thread;
 pthread_create(&thread,NULL, func, NULL);
 return 0;
}
