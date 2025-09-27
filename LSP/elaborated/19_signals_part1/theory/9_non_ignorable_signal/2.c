#include <signal.h>
#include <sys/signal.h>

int main(){

  //ignore SIGKILL ?
  signal(SIGKILL, SIG_IGN);

  //infinite loop
  while(1);

}
