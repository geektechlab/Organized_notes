#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	/* sending signal to self */
	if(raise(SIGSEGV) == 0) 
		printf("Successfully sent the signal\n");
	else
		printf("Failed to send signal\n");
	
	return EXIT_SUCCESS;
}
