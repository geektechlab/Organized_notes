#include <stdio.h>
#include <pthread.h>

struct test{
 int a;
 int b;
};

void *myThreadFn(void *arg)
{
 struct test *ptr = (struct test *)arg;
 sleep(1);
 printf("In Thread Fn with ptr->a:%d and ptr->b:%d\n",ptr->a, ptr->b);
 return NULL;
}

int main(int argc, char *argv[])
{
 struct test hello = {1,2};
 pthread_t tid;
 printf("Before Thread\n");
 pthread_create(&tid, NULL, myThreadFn,(void *) &hello);
 while(1) {}
 return 0;
}
