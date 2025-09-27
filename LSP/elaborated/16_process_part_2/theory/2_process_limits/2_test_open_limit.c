#include <stdio.h>
#include <fcntl.h>

/* this will print till limit set as ulimit ( hard or soft ? ) */

int main()
{
	int fd;
	int count = 0;
	while(1) 
	{
		fd = open("hello.txt", O_CREAT | O_WRONLY, 0666);
		if (fd < 0)
			break;
		count++;
		printf("count:%d\n", count);
	}
	
}

