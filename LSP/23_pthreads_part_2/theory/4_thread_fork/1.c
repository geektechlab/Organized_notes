#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

/* creating 2 childs and then calling a fork. it will not copy child threads to process and create those again */

pid_t child = -1;

void *threadfunc(void *arg) {
	printf("%d:%s starting up...\n", getpid(), (char *) arg);
	while (child == -1)
		sleep(2);
	printf("%d:%s finishing...\n",getpid(), (char*)arg);
	return NULL;  
}

int main() {
	pthread_t tid1, tid2;
	pthread_create(&tid1,NULL, threadfunc, "New Thread One");
	pthread_create(&tid2,NULL, threadfunc, "New Thread Two");
	printf("Parent PID:%d\n", getpid());
	child = fork();
	printf("%d:%s\n",getpid(), "fork()ing complete");
	sleep(2);
	printf("%d:%s\n",getpid(), "Main thread finished");
	pthread_exit(NULL);
}
