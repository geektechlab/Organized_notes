#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/io.h>

#define RTC_ADDRESS_PORT	0x70
#define RTC_DATA_PORT		0x71

unsigned char rtc_read(int offset)
{
	unsigned char value;

	ioperm(RTC_ADDRESS_PORT, 0x2, 1);

	outb(offset, RTC_ADDRESS_PORT);
        value = inb(RTC_DATA_PORT);

	ioperm(RTC_ADDRESS_PORT, 0x2, 0);
	return value;
}

int main(int argc, char **argv)
{
	int i;
	unsigned long tmp;

	unsigned char rtc_data[0x10];

	if (geteuid() != 0) {
		fprintf(stderr, "Must be root to run %s\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	for (i=0;i<sizeof(rtc_data); i++)
		rtc_data[i] = rtc_read(i);

	printf("RTC Current Time: %2.2x:%2.2x:%2.2x %2.2x/%2.2x/%2.2x \n",
			rtc_data[4],
			rtc_data[2],
			rtc_data[0],
			rtc_data[7],
			rtc_data[8],
			rtc_data[9]);

	exit(EXIT_SUCCESS);
}
