#include <signal.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>

void sighandler(int signum)
{
	printf("%s:PID:%d\n", __func__, getpid());
}

int main(){
   sigset_t newset, oldset;
   pid_t pid;

   printf("My PID:%d\n", getpid());
   signal(SIGINT, sighandler);
   sigemptyset(&newset);
   sigaddset(&newset, SIGINT);
   sigprocmask(SIG_SETMASK, &newset, &oldset);
   int i = 0;
   for(i=1; i<=8; i++){
	printf("I am masking SIGINT for 8 seconds, please press CTRL-C!\n");
	sleep(1);
  }
  pid = fork();
  printf("New process is created using fork()\n");
  sigprocmask(SIG_SETMASK, &oldset, NULL);
   for(i=1; i<=4; i++){
	printf("Now I am having the old sigset without any mask\n");
	sleep(1);
  }
  pause();
   return 0;
}

