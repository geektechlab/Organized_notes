#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[])
{
	int sockfd = socket (AF_UNIX, SOCK_DGRAM, 0);
	struct sockaddr_un dest_addr;

	dest_addr.sun_family = AF_UNIX;
	strcpy(dest_addr.sun_path, "./mysocket");

	char buff[128];
	write(2, "Enter data to sent:", 20);
	int n = read(0, buff, sizeof(buff));
	buff[n] = '\0';
	sendto(sockfd,buff, sizeof(buff), 0, (struct sockaddr*) &dest_addr, sizeof(dest_addr));
	close(sockfd);
	return 0;
}
