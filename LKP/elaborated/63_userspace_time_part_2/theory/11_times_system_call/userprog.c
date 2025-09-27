#include <stdio.h>		
#include <stdlib.h>		
#include <unistd.h>	
#include <stdint.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>

void spend_time()
{
	int fd;
	int i;
	long temp = 0;

	char buf[4096];

	//spend time in kernel
	if ((fd = open("/dev/urandom", O_RDONLY)) < 0) {
		perror("open");
		exit(1);
	}

	for (i=0; i < 50000L; i++) {
		read(fd, buf, sizeof(buf));
		memset(buf, 0, sizeof(buf));
	}

	close(fd);

	if ((fd = open("/dev/zero", O_RDONLY)) < 0) {
		perror("open");
		exit(1);
	}

	for (i=0; i < 50000L; i++) {
		read(fd, buf, sizeof(buf));
		memset(buf, 0, sizeof(buf));
	}

	close(fd);

	//spend time in user
	while (temp < UINT_MAX)
		temp++;
}

int main(int argc, char **argv)
{
	struct tms buf;
	long ticks_per_sec = 0;

	printf("tick frequency is: %lu\n", sysconf(_SC_CLK_TCK));
	ticks_per_sec = sysconf(_SC_CLK_TCK);

	spend_time();

	if (times(&buf) < 0) {
		perror("times");
		exit(1);
	}

	/* print the results */
	printf("user time: %lu ticks\t %lu seconds\n", buf.tms_utime,
		buf.tms_utime/ticks_per_sec);
	printf("system time: %lu ticks\t %lu seconds\n", buf.tms_stime,
		buf.tms_stime/ticks_per_sec);
	printf("chidren - user time: %lu ticks\t %lu seconds\n", buf.tms_cutime,
		buf.tms_cutime/ticks_per_sec);
	printf("chidren - system time: %lu ticks\t %lu seconds\n", buf.tms_cstime,
		buf.tms_cstime/ticks_per_sec);

	return 0;
}
