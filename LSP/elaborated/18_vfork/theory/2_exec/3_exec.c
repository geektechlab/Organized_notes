#include <stdio.h>
#include <unistd.h>

/* see both ids are same, in video at 23:17 */

int main()
{
	int ret;

	printf("PID before exec:%d\n", getpid());
	printf("PPID before exec:%d\n", getppid());
	printf("Calling exec\n");
	ret = execl("/home/linuxtrainer/Linux_System_Prog/day7_processes/1_process_ids/pid", "pid", NULL); /* replace the path with yours */
	if (ret == -1)
		perror("execl");
	printf("after exec \n");
	return 0;

}
