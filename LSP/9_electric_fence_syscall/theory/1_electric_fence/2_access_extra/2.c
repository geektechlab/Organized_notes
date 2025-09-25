#include <stdio.h>

int main()
{
    int *x = (int *) malloc(2*sizeof(int));
    x[3] = 5;   // Violation here!!!
    printf("Die\n");
}
