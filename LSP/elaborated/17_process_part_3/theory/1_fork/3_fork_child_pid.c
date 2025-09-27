#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

#define CHILD 0

/* run [ ps ] and check 2 processes got created */

int main()
{
	pid_t pid;
	printf(" My pid = %d \n", getpid());
	getchar();
	/* store child's pid */
	pid = fork();
	if( pid == CHILD){
		printf(" child: My pid = %d \n", getpid());
		while(1);
	}
	else{
		printf(" My pid = %d \n", getpid());
		printf(" Newly created child pid = %d \n", pid);
		while(1);
    }
}

