/*A process which has finished the execution but still has entry in the process table to report to its parent process is known as a zombie process.A child process always first becomes a zombie before being removed from the process table. The parent process reads the exit status of the child process which reaps off the child process entry from the process table.*/

#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h> 

int main() 
{ 
    // Fork returns process id 
    // in parent process 
    pid_t child_pid = fork(); 
  
    // Parent process  
    if (child_pid > 0) {
		printf("parent pid:%d\n", getpid());
        sleep(50); 
  	}
    // Child process 
    else {        
	/* this will become zombie process */
	/* verify it by running [ ps -ef ] and check it has become zombie using printed pid */
		printf("child pid:%d\n", getpid());
        exit(0); 
	}
  
    return 0; 
}
