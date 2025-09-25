#include <stdio.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int rtc_fd;
	int retval;
	int i = 0, count = 60, irq_count = 0;
	unsigned long data;	

	rtc_fd = open ("/dev/rtc", O_RDONLY);
        if (rtc_fd == -1)
        {
                perror("Cannot open /dev/rtc");
                return -1;
        }
	printf("Successfully opened /dev/rtc\n");

	retval = ioctl(rtc_fd, RTC_UIE_ON, 0);
	if (retval == -1)
	{
		perror("RTC_UIE_ON ioctl");
		exit(-1);
	}

	/* Reading periodic interrupts using read system call */
	for (i = 1; i <= count; i++)
	{
		retval = read(rtc_fd, &data, sizeof(unsigned long));
		if (retval == -1)
		{
			perror("read");
			exit(-1);
		}
		printf("periodic interrupt %d\n", irq_count);
		irq_count++;	
	}
	retval = ioctl(rtc_fd, RTC_UIE_OFF, 0);
	if (retval == -1)
	{
		perror("RTC_UIE_OFF ioctl");
		exit(-1);
	}

	close(rtc_fd);
	return 0;
}
