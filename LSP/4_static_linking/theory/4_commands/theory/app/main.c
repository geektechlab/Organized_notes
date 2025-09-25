/* #include "" looks in current folder */
/* #include <> looks in standard folder */

/* dynamic linking: gcc main.c -o main -L<path to directory of arith.h> -larith -L<path to directory of libarith.a> -v */
/* static linking: gcc main.c -o main -L<path to directory of arith.h> -larith -L<path to directory of libarith.a> --static -v */

#include "arith.h"
#include <stdio.h>
 
int main()
{
  int x= 10, y = 20;
  printf("\n%d + %d = %d", x, y, add(x, y));
  printf("\n%d - %d = %d", x, y, sub(x, y));
  printf("func:%d\n", func());
  return 0;
}
