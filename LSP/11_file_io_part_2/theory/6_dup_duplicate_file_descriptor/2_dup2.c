#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/* it shows redirection of printf to file instead of stanard console output. So, we can write to file just using printf functionalities */

int main(void)
{
	// Create a new file for writing only. Truncate if it exits.
	int file = open("dup2.temp", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	if(file == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	// Make it so writing to file
	/* here we are giving STDOUT_FILENO to dup2. so first it will close STDOUT_FILENO meaning standard output. so now STDOUT_FILENO will write to file instaed of standard output. */
	if(dup2(file, STDOUT_FILENO) == -1) // STDOUT_FILENO is 1.
	{
		perror("dup");
		exit(EXIT_FAILURE);
	}

	// Won't print to terminal! Will write to file instead.
	printf("Hello world\n");
	
	// Write message to stderr (because messages written to stdout won't appear!
	fprintf(stderr, "Look in dup2.temp\n");

	return 0;
}
