#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
	printf("I'm printed once!\n");
	fork();
	// Now there are two processes running
// and each process will print out the next line.
	printf("You see this line twice!\n");
	return 0;
}

