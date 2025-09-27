#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h> /*for signal() and raise()*/

void hello(int signum){
	volatile unsigned long long i;
	printf("start of signal handler\n");
	for (i = 0; i < 2000000000ULL; ++i);
	printf("end of signal handler\n");
}

int main(){

  //Handle SIGINT with hello
  signal(SIGINT, hello);

  //loop forever!
  while(1) {
	sleep(1);
  }

}
