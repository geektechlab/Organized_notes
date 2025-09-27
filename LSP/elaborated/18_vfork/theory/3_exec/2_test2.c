
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(){
	printf(" standard output\n");
	perror("standard error");
}
