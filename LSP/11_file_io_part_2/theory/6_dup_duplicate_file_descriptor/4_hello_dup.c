#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

/* write content in file using STDOUT file desriptor, but close call is not needed here because dup2 anyway closes it. */

int main(int argc, char * argv[]){
  //print hello world to a file with dup
  int fd;

  //check args
  if ( argc < 2){
    fprintf(stderr, "ERROR: Require destination path\n");
    return 1;
  }

  //open destination file
  if( (fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT , 0644)) < 0){
    perror("open");
    return 1;
  }

  //close standard out
  /* it is not required because dup2 will close it anyway */
  close(1);

  //duplicate fd to stdout
  dup2(fd, 1);

  //print to stdout, which is now duplicated to fd
  printf("Hello World!\n");
  printf("value = %d\n", 5);
  write(fd, "Linux", 5);
  /* since printf is buffered and write is unbuffered Linux will get added in file first and then printf content will come. We can verify it using [ strace hello_dup ] */

  return 0;
}
