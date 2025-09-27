#include <stdio.h>
#include <unistd.h>

/* it runs vi and opens vi with argument */

int main()
{
	int ret;

	//vi is passed as the first argument so a program can examine its first argument argv[0] to discover the 
	//name of its binary image
	ret = execl("/usr/bin/vi", "vi", "/home/linuxtrainer/Linux_System_Prog/day9_processes/prenotes.txt", NULL);
	if (ret == -1)
		perror("execl");

}
