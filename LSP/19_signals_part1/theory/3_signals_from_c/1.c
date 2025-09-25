#include <stdlib.h>
#include <stdio.h>

/* check [ man signal ] and [ man raise ] */

#include <signal.h> /*for signal() and raise()*/

void hello(int signum){
  printf("Hello World!\n");
}

int main(){

  //execute hello() when receiving signal SIGUSR1  
  signal(SIGUSR1, hello);

  //send SIGUSR1 to the calling process  
  raise(SIGUSR1);
}
