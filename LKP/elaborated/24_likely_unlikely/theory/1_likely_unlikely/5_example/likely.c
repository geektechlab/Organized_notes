#include <stdio.h>

#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

void foo()
{
	printf("foo\n");
}

void bar()
{
	printf("bar\n");
}

int main(int argc, char *argv[])
{
	int i = atoi(argv[1]);

	if (likely(i < 0))
		foo();
	bar();
	return 0;
}
