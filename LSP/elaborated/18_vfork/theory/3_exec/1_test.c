#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(){
	int fd = open("./1.c",O_RDONLY);
	printf("in test3 fd = %d\n",fd);
	close(fd);
	return 0;
}
