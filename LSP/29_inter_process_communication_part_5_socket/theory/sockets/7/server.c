#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>


int main()
{
	const char *SOCK_PATH = "/tmp/srv_socket" ;
	int sock_fd, client_fd;
	struct sockaddr_un addr, client_addr;
	socklen_t addrlen;
	char buffer[1000];

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
	}
	printf("Bind successful\n");

	listen(sock_fd, 5);
	perror("listen");

	client_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &addrlen);
       	perror("accept");	
	printf("accept client filedescriptor:%d\n", client_fd);
	int sc;
	struct ucred ucred;
	socklen_t len;
	sc = getsockopt(client_fd, SOL_SOCKET, SO_PEERCRED, &ucred, &len);
	if (sc < 0) {
		fprintf(stderr,"getsockopt: %s\n", strerror(errno));
		exit(1);
	}

	fprintf(stderr, "Credentials from SO_PEERCRED: pid=%ld, euid=%ld, egid=%ld\n",
    (long) ucred.pid, (long) ucred.uid, (long) ucred.gid);

	read(client_fd, buffer, sizeof(buffer));
	perror("read");
	printf("buffer from client:%s\n", buffer);
	close(client_fd);
	close(sock_fd);

	return 0;
}
