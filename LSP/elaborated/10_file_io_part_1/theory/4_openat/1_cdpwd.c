# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <limits.h>

int main(int argc , char * argv[])
{
	char *ptr;
	int maxpathsize;
	maxpathsize = pathconf("/",_PC_PATH_MAX);
	ptr = (char *) malloc (maxpathsize);
	/* getcwd() is library call [ man 3 getcwd ] */
	/* gives current working directory */
	if (getcwd(ptr, maxpathsize) == NULL)
	{
		perror("prog:");
		exit(1);
	}
	printf("cwd = %s\n", ptr);
	/* getcwd() is system call [ man 2 getcwd ] */
	/* changes current working directory. It changes current working directory of a process, not the shell where it is getting executed */
	if (chdir(argv[1]) < 0)
	{
		perror("prog:");
		exit(1);
	}
	if (getcwd(ptr, maxpathsize) == NULL)
	{
		perror("prog:");
		exit(1);
	}
	printf("cwd = %s\n", ptr);
}
