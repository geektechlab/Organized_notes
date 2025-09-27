#include<stdio.h> 
  
// Assigning functions to be executed before and after main
void __attribute__((constructor)) constructor(); 
void __attribute__((destructor)) destructor(); 
  
int main() 
{ 
    printf("%s\n", __func__); 
	return 0;
} 
  
void constructor() 
{ 
    printf("%s\n", __func__); 
} 
  
void destructor() 
{ 
    printf("%s\n", __func__); 
} 
