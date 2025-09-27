#include "arith.h"
#include <stdio.h>
 
int main()
{
	int x= 10, y = 20;
	printf("%s\n",__func__);
	printf("\n%d + %d = %d", x, y, add(x, y));
	return 0;
}
