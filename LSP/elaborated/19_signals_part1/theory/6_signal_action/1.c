#include <stdlib.h>
#include <stdio.h>

#include <signal.h> /*for signal() and raise()*/

void nothing(int signum){ /*DO NOTHING*/ }

int main(){

  signal(SIGINT, nothing);

  while(1);
}

