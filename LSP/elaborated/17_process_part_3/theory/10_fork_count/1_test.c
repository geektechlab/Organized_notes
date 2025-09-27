/*This program will create ____ child processes?*/

#include<stdio.h>
#include<unistd.h>

int main()
{
	fork(); /* 2 processes now */
	fork(); /* 4 processes now */
	fork(); /* 8 processes now */
	printf("embedded\n"); /* will get printed 8 times */
	return 0;
}
