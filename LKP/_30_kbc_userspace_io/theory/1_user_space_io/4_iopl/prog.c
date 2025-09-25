#include <stdio.h>
#include <sys/io.h>

int main()
{
	outb(0x00, 0x70);
	printf("%d\n", inb(0x71));
	return 0;
}
