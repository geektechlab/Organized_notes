#include <stdio.h>

/* analyse it with core dump */

int main()  
{
	int *ptr = NULL; // NULL pointer.  
	*ptr = 10;    // This is invalid assignment.

	printf("%s", "SW-VERSION=0.1");  
	return 0;  
}
