#include <stdio.h> 

/* run [ size ./2_prog1 ] to find components within program */

int global; /* Uninitialized variable stored in bss*/
  
int main(void) 
{ 
	static int i;
	global++;
    return 0; 
} 
