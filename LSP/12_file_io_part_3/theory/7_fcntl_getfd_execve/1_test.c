# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <fcntl.h>

main(){
	printf(" standard output\n");
	perror("standard error\n");
}
