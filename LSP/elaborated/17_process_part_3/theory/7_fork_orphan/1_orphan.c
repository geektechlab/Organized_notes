/*A process whose parent process no more exists i.e. either finished or terminated without waiting for its child process to terminate is called an orphan process.
*/

#include<stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
  
int main() 
{ 
    // Create a child process       
    int pid = fork(); 
  
    if (pid > 0) 
        printf("in parent process:%u\n", getpid()); 
  
    // Note that pid is 0 in child process 
    // and negative if fork() fails 
	/* this will become orphan process */
	/* verify it by running [ ps -ef | grep <process id> | less ] and check it has become orphan using printed pid */
	/* pid of systemd gets assigned to all orphan processes */

    else if (pid == 0) 
    { 
        sleep(30); 
        printf("in child process"); 
    } 
  
    return 0; 
} 
