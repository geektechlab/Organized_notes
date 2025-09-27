#include <stdio.h>
#include <unistd.h>

int main()
{
	int ret;
	char *const args[] = { "vi", "/home/linuxtrainer/Linux_System_Prog/day9_processes/prenotes.txt", NULL};

	///usr/bin should be in the vim path
	ret = execvp("vi", args);
	if (ret == -1)
		perror("execvp");
}
