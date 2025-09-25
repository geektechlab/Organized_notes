#include <stdio.h> 

extern void func(void); 

int main(void) 
{ 
    printf("\n Inside main()\n"); 
    func(); 
    return 0; 
}
