#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void no_one_listening(int signal) {
    write(1, "No one is listening!\n", 21);
}

int main() {
    signal(SIGPIPE, no_one_listening);
    int filedes[2];
    char buf[10];
    
    pipe(filedes);
    pid_t child = fork();
    if (child > 0) { 
	close(0);
	dup(filedes[0]);
        close(filedes[0]);
	close(filedes[1]);
	scanf("%s", buf);
	printf("buffer:%s\n", buf);
    } else {
	close(filedes[0]);
	write(filedes[1], "linux", 5);
	close(filedes[1]);
    }
    return 0;
}
