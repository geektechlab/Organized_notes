#include <unistd.h>
#include <stdlib.h>
#include <sys/io.h>

#define I8042_DATA_REG          0x60
#define I8042_STATUS_REG        0x64

void push_keycode(unsigned char code)
{
        while (inb(I8042_STATUS_REG) & 0x2) 
		;
	outb(0xd2, I8042_STATUS_REG);
        while (inb(I8042_STATUS_REG) & 0x2) 
		;
	outb(code, I8042_DATA_REG);
}

/*  keycodes in http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html */
unsigned char hello[] = { 
	0x23, 0x12, 0x26, 0x26, 0x18, 
	0x26, 0x17, 0x31, 0x16, 0x2d, 0
};

int main(int argc, char **argv)
{
	unsigned char *ptr = hello;

	iopl(3);

	for (ptr = hello; *ptr; ptr++) {
		push_keycode(*ptr);		/* Key down */
		sleep(1);
		push_keycode(*ptr | 0x80);	/* Key up */
		sleep(1);
	}

	exit(EXIT_SUCCESS);
}
