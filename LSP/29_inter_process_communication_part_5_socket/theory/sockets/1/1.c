#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

int main()
{
	const char *SOCK_PATH = "/tmp/srv_socket" ;
	int sock_fd;
	struct sockaddr_un addr;

	sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock_fd < 0) {
		perror("socket");
		exit(-1);
	}

	printf("Socket File Descriptor:%d\n", sock_fd);

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	//copying one byte less for trailing zero
	strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path)-1);
	
	if (bind(sock_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) < 0) {
		perror("bind failed");
		exit(1);
	}
	printf("Bind successful\n");

	return 0;
}
