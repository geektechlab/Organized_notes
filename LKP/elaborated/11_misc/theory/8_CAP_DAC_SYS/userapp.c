/* run [ setcap cap_dac_override+ep ./userapp
getcap ./userapp
run [ setcap cap_dac_override-ep ./userapp
getcap ./userapp
*/

#include <stdio.h>
#include <sys/fcntl.h>

int main()
{
	int fd;

	fd = open("/etc/shadow", O_RDONLY);
	perror("fd");
	return 0;
}
