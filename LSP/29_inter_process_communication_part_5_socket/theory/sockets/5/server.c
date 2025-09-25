#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define UNIX_PATH_MAX 108

/*
 * Buffer we send to the server.
 */
char buffer[100] ;

int main()
{
    int connection_fd;
    int sock_fd= -1, i, address_len;

    /* Unix Sock Address structure */
    struct sockaddr_un server_address;

    /*
     * Create domain socket.
     * This socket will be in the UNIX domain,
     * and will be a stream socket.
     */

    if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("server: socket");
        return -1;
    }

    /* start with a clean address structure */
    memset(&server_address, 0, sizeof(struct sockaddr_un));

    /*
     * Create the address we will be connecting to.
     */
    server_address.sun_family = AF_UNIX;
    snprintf(server_address.sun_path, UNIX_PATH_MAX, "my_socket");

    if(bind(sock_fd, 
	    (struct sockaddr *) &server_address, 
	    sizeof(struct sockaddr_un)) != 0)
    {
	    printf("bind() failed\n");
	    return 1;
    }

    /*
     * Listen on the socket.
     */
    if( listen(sock_fd, 5) != 0)
    {
	    printf("listen() failed\n");
	    return 1;
    }

    /*
     * Accept connections.  When we accept one, ns
     * will be connected to the client.  fsaun will
     * contain the address of the client.
     */
    while (1)
    {
       int done, len;

       connection_fd = accept(sock_fd,
             (struct sockaddr *)&server_address,
             &address_len);
       if(connection_fd < 0)
       {
         perror("server: accept");
         return -1;
       }


       /* Recv and Send in Infinite Loop */
       done = 0;
       do
       {
          /* Recieve Data from Client */
          len = recv(connection_fd, buffer, 100, 0);

          if (len <= 0)
          {
             if (len < 0)
                perror("recv");
             done = 1;
          }

          if (!done)
             /* Send Data to Client */
             if (send(connection_fd, buffer, len, 0) < 0)
              {
                  perror("send");
                  done = 1;
              }
        } while (!done);

      /*
       * We can simply use close() to terminate the
       * connection, since we're done with both sides.
       */

      close(connection_fd);
    }
    return 0;
}
