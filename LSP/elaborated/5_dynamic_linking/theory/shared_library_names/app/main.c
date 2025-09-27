#include "arith.h"
#include <stdio.h>
extern int func();
int main()
{
	int x= 10, y = 20;
	printf("%s\n",__func__);
	printf("func:%d\n", func());
	printf("\n%d + %d = %d", x, y, add(x, y));
	return 0;
}
