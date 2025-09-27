//hello world program

/* Run [ gcc -E hello.c > hello.i ] and -E will stop after preprocessing. Check using [ cat hello.i ] */
/* Run [ gcc -S hello.i > hello.s ] and -S will stop after compilation. Check using [ cat hello.s ] */
/* Run [ gcc -c hello.s > hello.o ] and -c will stop after assembler. we can't use [ cat hello.o ] since it is object file. check txt file to view content of .o file */
#include <stdio.h>

#define STRING "Hello World!\n"
 
int main() 
{
    printf(STRING);
    return 0;
}
