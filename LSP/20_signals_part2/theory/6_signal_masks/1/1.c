#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int main()
{
	/* define a new mask set */
	sigset_t mask_set;
	/* first clear the set (i.e. make it contain no signal numbers) */
	sigemptyset(&mask_set);

	/* just creating a set of signals in mask */
	sigaddset(&mask_set, SIGINT);
	sigaddset(&mask_set, SIGQUIT);

	if (sigismember(&mask_set, SIGINT))
		printf("signal INT is in our set\n");
	else
		printf("signal INT is not in our set - how strange...\n");

	if (sigismember(&mask_set, SIGQUIT))
		printf("signal QUIT is in our set\n");
	else
		printf("signal QUIT is not in our set - how strange...\n");

	sigdelset(&mask_set, SIGQUIT);
	if (sigismember(&mask_set, SIGQUIT))
		printf("signal QUIT is in our set\n");
	else
		printf("signal QUIT is not in our set - how strange...\n");
	return 0;
}
