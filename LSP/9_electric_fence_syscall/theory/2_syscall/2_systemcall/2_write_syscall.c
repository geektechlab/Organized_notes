#include <unistd.h>

/* run [ make 2_write_syscall ] which is shortcut for [ cc 2_write_syscall.c -o 2_write_syscall ] */
/* then run [ starce ./2_write_syscall ] */
/* we can see prints uses write system call and it uses 1 which is standard output */

int main(){
   char str[]={"Welcome to System Programming...\n"};
    /* try uncommenting and check which system call it uses by strace */
	//printf("strlen:%d\tsizeof:%d\n", strlen(str), sizeof(str));
	int rv = write(1, str, sizeof str); 
   return 0;
}

