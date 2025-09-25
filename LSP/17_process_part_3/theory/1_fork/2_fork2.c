/*
	The following program prints out 42 twice - but the fork() is after the printf!? Why?
	this progra is not valid now for latest linux kernel. We can skip this program
*/

#include <unistd.h> /*fork declared here*/
#include <stdio.h> /* printf declared here*/
int main() {
   int answer = 84 >> 1;
   printf("Answer: %d", answer);
   fork();
   return 0;
}
