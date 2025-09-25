#include <stdio.h>
#include <sys/io.h>
#include <unistd.h>

#define SPEAKER_PORT 0x61
#define TIMER2_PORT  0x42
#define PIT_SETTING_PORT 0x43
#define PIT_BASE_PORT 0x40

int main()
{
	int freq;
	ioperm(PIT_BASE_PORT, 4, 0x01);
	ioperm(SPEAKER_PORT, 0x01, 0x01);

	for (freq = 100 ; freq < 800; freq+=100) {
		unsigned short div = 1193181/freq; //frequency is 1.193 MHz
		
		outb(0xb6, PIT_SETTING_PORT);
		outb(div, TIMER2_PORT);
		outb(div>>8, TIMER2_PORT);
		outb(0x03, SPEAKER_PORT); //Turn ON Speaker
		sleep(1);
	}

	outb(0x00, SPEAKER_PORT); //Turn OFF Speaker
	return 0;
}
