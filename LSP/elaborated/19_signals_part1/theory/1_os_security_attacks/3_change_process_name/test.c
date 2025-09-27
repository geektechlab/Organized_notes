#include <stdio.h>
#include <unistd.h>
#include <string.h>

/* check updated process name by [ ps -ef ] */

int main(int argc, char *argv[])
{
	printf("hello world\n");
	if (argc == 2)
		strcpy(argv[0] , argv[1]);
	pause();
}
