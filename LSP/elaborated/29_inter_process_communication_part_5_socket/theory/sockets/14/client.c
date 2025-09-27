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

	char buff[20] = "Hello from Client..";
	sendto(sockfd,buff, sizeof(buff), 0, (struct sockaddr*) &dest_addr, sizeof(dest_addr));
	close(sockfd);
	return 0;
}
