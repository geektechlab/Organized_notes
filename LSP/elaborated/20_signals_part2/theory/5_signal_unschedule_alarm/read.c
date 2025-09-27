#include <stdio.h>
#include <unistd.h>

int main()
{
	char buf[10];

	read(0, buf, sizeof(buf));
	perror("read");
	printf("buf:%s\n", buf);
	return 0;
}
