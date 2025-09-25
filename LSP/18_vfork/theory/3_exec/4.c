#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(){
	printf("Can you see me [ONE]\n");
	execl("./test","progname","arg1","arg2",NULL);
	/* this will not get printed because address space of test is loaded and this program's address space is removed */
	printf("Can you see me [TWO]\n");
	return 0;
}
