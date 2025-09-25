#include <stdio.h>
#include <unistd.h>

/* it runs vi and opens vi */
/* actually when we run vi, in background this happens */

int main()
{
	int ret;

	//vi is passed as the first argument so a program can examine its first argument argv[0] to discover the 
	//name of its binary image
	ret = execl("/usr/bin/vi", "vi", NULL);
	if (ret == -1)
		perror("execl");

}
