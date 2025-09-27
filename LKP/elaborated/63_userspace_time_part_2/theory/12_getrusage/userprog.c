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
#include <sys/resource.h>

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
	struct rusage usage;

	spend_time();
	getrusage(RUSAGE_SELF, &usage);

	/* print the results */
	printf(" User time\t: seconds:%lu\t microseconds:%lu\n",
		usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);

	printf(" Kernel time\t: seconds:%lu\t microseconds:%lu\n",
		usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);

	return 0;
}
