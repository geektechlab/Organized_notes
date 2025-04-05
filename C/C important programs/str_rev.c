/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>

void str_rev(char *str, size_t size)
{
	// think why -2 is used, not -1
    int start=0, end=size-2;
    while(start<end)
    {
        (*(str+start))^=(*(str+end));
        (*(str+end))^=(*(str+start));
        (*(str+start))^=(*(str+end));
        start++;
        end--;
    }
}

int main()
{
    char source[] = "HelloHello";

    str_rev(source, sizeof(source));
    printf("%s\r\n",source);

    return 0;
}