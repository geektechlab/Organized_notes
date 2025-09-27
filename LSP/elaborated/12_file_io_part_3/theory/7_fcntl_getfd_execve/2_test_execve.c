# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <fcntl.h>

main(){
	/* try uncommening following lines and see what happens with prints, following lines define what to do with file descriptor after execve */
	//fcntl(0,F_SETFD,FD_CLOEXEC) ; //set close on exec flag
	//fcntl(1,F_SETFD,FD_CLOEXEC) ; 
	//fcntl(2,F_SETFD,FD_CLOEXEC) ; 
	execve("./test",NULL, NULL);	
	printf("Can you see me [TWO]\n");
}
