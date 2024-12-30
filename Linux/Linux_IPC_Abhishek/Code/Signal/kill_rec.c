/* compile: gcc -g -c kill_rec.c -o kill_rec.o
   link:    gcc -g kill_rec.o -o kill_rec -lrt
   we can run both processes in single shell using daemon process. run ./kill_rec& to create daemon */

/* Run this program as a daemon and get process id using ps command. Feed that process id to kill_sender process and observe output */

#include <stdio.h>
#include <signal.h>

static void signal_handler(int sig)
{
    printf("Signal=%d received\r\n",sig);
}

int main(int argc, char **argv)
{
    signal(SIGUSR1, signal_handler);
    while(1){}

    return 0;
}
