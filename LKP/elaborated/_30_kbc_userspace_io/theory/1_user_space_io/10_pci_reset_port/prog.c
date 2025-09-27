#include <stdio.h>
#include <sys/io.h>

#define PCI_RESET_PORT	0xcf9

int main()
{
	ioperm(PCI_RESET_PORT, 0x01, 0x01);
	outb(0x02, PCI_RESET_PORT);
	usleep(100);
	outb(0x04, PCI_RESET_PORT);
	return 0;
}
