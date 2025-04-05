/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>

void mem_move(char *dst, char *src, size_t size)
{
    if(dst>src)
    {
        while(size--)
        {
            *(dst+size)=*(src+size);
        }
    }
    else
    {
        size_t iter = 0;
        while(iter!=size)
        {
            *(dst+iter)=*(src+iter);
            iter++;
        }
    }
}

int main()
{
    char source[] = "HelloHello";

    mem_move((source+2), source, 2);
    printf("%s\r\n",source);

    mem_move((source-3), source, 4);
    printf("%s\r\n",source);

    return 0;
}