
#include <stdio.h> 
  
int global = 10; 
  
int main(void) 
{ 
	static int i = 4;
	global++;
	printf("hello world\n");
    return 0; 
} 
