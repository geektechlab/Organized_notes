#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>

/* here we are opening file descriptor in other process. But other process will not get that opened file descriptor because if it would have been opened before forking then it would have been fine. But after forking, openining a file descriptor will not reflect in other. So, child read call will fail */

#define CHILD 0

main(){
	int fd = 10;
	char buf[2];
	pid_t pid;	
	int childstatus;
	pid = fork();
	if( pid == CHILD){
		sleep(4);
		read(fd,buf,2);
		perror("child:");
		printf(" in child %c\n",buf[0]);
		printf(" in child %c\n",buf[1]);
		close(fd);
	}
	// parent
	else{
		fd = open("./test",O_RDONLY);
		printf("parent waiting for child\n");
		wait(&childstatus);
		read(fd,buf,2);
		printf(" in parent %c\n",buf[0]);
		printf(" in parent %c\n",buf[1]);
		close(fd);
	}
}

