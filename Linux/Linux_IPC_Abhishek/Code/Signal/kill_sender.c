/* compile: gcc -g -c kill_sender.c -o kill_sender.o
   link:    gcc -g kill_sender.o -o kill_sender -lrt
   we can run both processes in single shell using daemon process. run ./kill_sender& to create daemon */

/* Provide process id of kill_rec process in first argument and observe output */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    kill(atoi(*(argv+1)), SIGUSR1);
    while(1){}

    return 0;
}
