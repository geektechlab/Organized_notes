#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

/* here we are not calling glibc wrapper but are using system call directly */
/* run [ man syscall ] for more details */
/* first argumnet to system call is a number and remaining arguments are which are needed by and are specific to system call. All system call numbers can be found in <sys/syscall.h> */
/* We can find SYS_write in [ cd /usr/include | grep -rin --color 'SYS_write' ] */

int main(){
   char str[]={"Welcome to System Programming\n"};
//   int rv = syscall(SYS_write, 1, str, sizeof str); 
   int rv = syscall(1, 1, str, sizeof str); 
   return rv;
}

