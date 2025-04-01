/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>

#define SIZEOF(var) (((char *)((&(var))+1)) - (char *)(&(var)))

int str_len(char *str)
{
    int i=0, count = 0;
    while(*(str+i)!='\0')
    {
        count++;
        i++;
    }
    return count;
}

int main()
{
    char source[] = "HelloHello";
    char replace[] = "This this";
    char str_size[10] = "size";
    
    printf("%d %d\r\n",str_len(source),str_len(replace));
    printf("%d %d\r\n",str_len(str_size),SIZEOF(str_size));

    return 0;
}