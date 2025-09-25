#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main()
{
	int sockfd;
	struct sockaddr_un server_addr;
	char buff[10];
	int addr_len = sizeof(server_addr);

	sockfd = socket (AF_UNIX, SOCK_DGRAM, 0);
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, "./mysocket");

	bind(sockfd, (struct sockaddr*)&server_addr, sizeof server_addr);
	perror("bind");
	int i = 2;
	while (i--)
	{
		recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr*)&server_addr, &addr_len);
		perror("recvfrom");

		printf("Recevied from client:%s\n", buff);
	}
	close(sockfd);
	return 0;
}

