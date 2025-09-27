#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* run ./1_hello and find return value by [ echo $? ] */
/* here return 0; will just return 0 and not terminate program but exit will return 0 and terminate program as well */
/* run [ man exit ] and find it terminates all threads of program not just one thread */

int func()
{
	//return 0;
	exit(0);
}

int main(int argc, const char *argv[])
{
	printf("%s\n", "hello world");
	func();
	printf("printing\n");
	//return 0; 
	exit(0);

}
