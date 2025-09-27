#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

char *argstochild[10];
char *envofchild[10];

void prepare_args(){
	argstochild[0] = (char *) malloc(20);
	strcpy(argstochild[0],"./progname"); 
	argstochild[1] = (char *) malloc(20);
	strcpy(argstochild[1],"param1"); 
	argstochild[2] = (char *) malloc(20);
	strcpy(argstochild[2],"param2"); 
	argstochild[3]=NULL;
}

void prepare_env(){
	envofchild[0] = (char *) malloc(20);
	strcpy(envofchild[0],"env1=value1"); 
	envofchild[1] = (char *) malloc(20);
	strcpy(envofchild[1],"env2=value2"); 
	envofchild[2]=NULL;
}
int main(){
	fcntl(0,F_SETFD,FD_CLOEXEC) ; //set close on exec flag
	fcntl(1,F_SETFD,FD_CLOEXEC) ; 
	fcntl(2,F_SETFD,FD_CLOEXEC) ; 
	prepare_args();
	prepare_env();
	execve("./2_test2",argstochild,envofchild);	
	printf("Can you see me [TWO]\n");
}
