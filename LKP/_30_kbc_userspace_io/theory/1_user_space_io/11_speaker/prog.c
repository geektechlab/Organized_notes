#include <stdio.h>
#include <sys/io.h>
#include <unistd.h>

#define SPEAKER_PORT 0x61

int main()
{
	ioperm(SPEAKER_PORT, 0x01, 0x01);
	outb(0x03, SPEAKER_PORT); //Turn ON Speaker
	sleep(3);
	outb(0x00, SPEAKER_PORT); //Turn OFF Speaker
	return 0;
}
