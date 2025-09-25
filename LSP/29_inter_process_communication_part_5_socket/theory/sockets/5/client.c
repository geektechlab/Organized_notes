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
    int sock_fd= -1, i, len;

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

     /*
     * Connect to the address.  For this to
     * succeed, the server must already have bound
     * this address, and must have issued a listen()
     * request.
     *
     * The third argument indicates the "length" of
     * the structure.
     *
     */

    if(connect(sock_fd,
            (struct sockaddr *) &server_address,
            sizeof(struct sockaddr_un)) != 0)
    {
	    printf("connect() failed\n");
	    return 1;
     }

    /* Send and recieve Data
     *   in while Loop
     */
    while(printf("> "), fgets(buffer, 100, stdin), !feof(stdin))
    {
        /* Send the Data to server */
        if (send(sock_fd, buffer, strlen(buffer), 0) == -1)
         {
            perror("send");
            return 1;
         }

        /* Recieve the Data from Server */
        if ((len=recv(sock_fd, buffer, 100, 0)) > 0)
         {
            buffer[len] = '\0';
            printf("echo> %s", buffer);

         }
        else
         {
            if (len < 0)
              perror("recv");
            else // Length = 0
              printf("Server closed connection\n");
              return 1;
        }
     }

    /*
     * We can simply use close() to terminate the
     * connection, since we're done with both sides.
     */
    close(sock_fd);

    return 0;

} 
