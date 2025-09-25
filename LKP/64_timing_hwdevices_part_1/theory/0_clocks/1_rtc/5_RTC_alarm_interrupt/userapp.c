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
	int i = 0, count = 600, irq_count = 0;
	unsigned long data;
	struct rtc_time alarm;	
	int sec = 30;

	rtc_fd = open ("/dev/rtc", O_RDONLY);
        if (rtc_fd == -1)
        {
                perror("Cannot open /dev/rtc");
                return -1;
        }
	printf("Successfully opened /dev/rtc\n");
	ioctl (rtc_fd, RTC_RD_TIME, &alarm);
        printf ("\nCurrent Time: %02d:%02d:%02d", alarm.tm_hour, alarm.tm_min, alarm.tm_sec);
        printf ("\nSetting Alarm After %d Seconds", sec);
	/* Add sec to current time */
        alarm.tm_sec += sec;
        /* Propagate carry to tm_min */
        if (alarm.tm_sec >= 60)
        {
                alarm.tm_min += alarm.tm_sec / 60;
                alarm.tm_sec %= 60;
        }
	        /* Propage carry to tm_hour */
        if (alarm.tm_min >= 60)
        {
                alarm.tm_hour += alarm.tm_min / 60;
                alarm.tm_min %= 60;
        }

        /* Wrap tm_hour */
        if (alarm.tm_hour == 24)
        {
                alarm.tm_hour %= 24;
        }

	/* Set the Alarm time */
        printf ("\nSetting Alarm Time To : %02d:%02d:%02d",
                        alarm.tm_hour, alarm.tm_min, alarm.tm_sec);
        ioctl (rtc_fd, RTC_ALM_SET, &alarm);

	ioctl (rtc_fd, RTC_ALM_READ, &alarm);
        printf ("\nAlarm Time Set To : %02d:%02d:%02d",
                        alarm.tm_hour, alarm.tm_min, alarm.tm_sec);

        printf ("\nWaiting for Alarm ...");
        fflush (stdout);
	
	retval = ioctl(rtc_fd, RTC_AIE_ON, 0);
	if (retval == -1)
	{
		perror("RTC_AIE_ON ioctl");
		exit(-1);
	}

	retval = read(rtc_fd, &data, sizeof(unsigned long));
	if (retval == -1)
	{
		perror("read");
		exit(-1);
	}
	printf("Alarm....Alarm....Alarm....\n");
	retval = ioctl(rtc_fd, RTC_AIE_OFF, 0);
	if (retval == -1)
	{
		perror("RTC_AIE_OFF ioctl");
		exit(-1);
	}

	close(rtc_fd);
	return 0;
}
