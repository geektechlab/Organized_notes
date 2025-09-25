#include<stdio.h>

int main()
{
	int fd[2];
	char buff[11];
	if (pipe(fd) != 0)
		perror("pipe");
	write(fd[1],"Hello World",11);
	lseek(fd[0],0,6);
	perror("lseek");
	read(fd[0],buff,11);
	printf("%s\n",buff);
	return 0;
}

