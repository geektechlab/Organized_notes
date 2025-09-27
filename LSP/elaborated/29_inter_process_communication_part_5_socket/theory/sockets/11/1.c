#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, char **argv)
{
	int s;
	struct sockaddr_un sun;

	if (argc != 2 || strlen(argv[1]) + 1 > sizeof(sun.sun_path)) {
		fprintf(stderr, "usage: %s abstract-path\n", argv[0]);
		exit(1);
	}

	s = socket(AF_UNIX, SOCK_STREAM, 0);
	if (s < 0) {
		perror("socket");
		exit(1);
	}
	memset(&sun, 0, sizeof(sun));
	sun.sun_family = AF_UNIX;
	strcpy(sun.sun_path + 1, argv[1]);
	if (bind(s, (struct sockaddr *) &sun, sizeof(sun))) {
		perror("bind");
		exit(1);
	}
	pause();
}
