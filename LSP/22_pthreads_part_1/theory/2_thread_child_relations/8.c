//What happens if a thread tries to join upon itself

/* it will fail with error number. run [ errno <error number> ] and then find that deadlock is happening */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	int ret;
	if ((ret = pthread_join(pthread_self(), NULL)) != 0) {
		printf("pthread_join failed with ret:%d\n", ret);
	}
	printf("%s:%d\n",__func__, getpid());
	return 0;
}

