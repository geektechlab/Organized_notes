/*
	A 'fork bomb' is when you attempt to create an infinite number of processes. 
*/

/* run [ ps ] and check lot of fork are being created */

/* once user process limit from [ ulimit -a ] gets exhausted, it will stop/crash ? */

#include <unistd.h> /*fork declared here*/
#include <stdio.h> /* printf declared here*/
int main() 
{
	while(1)
	{
		fork();
		perror("fork()");
	}

		return 0;
}
