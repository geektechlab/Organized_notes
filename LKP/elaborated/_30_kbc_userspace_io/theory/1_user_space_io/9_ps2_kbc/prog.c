#include <stdio.h>
#include <sys/io.h>

#define KEYBOARD_PORT 0x64

int main()
{
	ioperm(KEYBOARD_PORT, 0x01, 0x01);
	outb(0xfe, KEYBOARD_PORT);
	return 0;
}
