#include <unistd.h>
#include <stdio.h>
#include <signal.h>

int pleaseStop;

void handle_sigint(int signal) {
    pleaseStop = 1;
}

int main() {
    signal(SIGINT, handle_sigint);
    pleaseStop = 0;
    while (! pleaseStop) { 
        /* application logic here */ 
    }
    /* cleanup code here */
	printf("signal handler received\n");
}
