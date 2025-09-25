#include "stdio.h"

int main() {
    char ch = getchar();
    if (__builtin_expect(ch == 0 , 0))
        printf("not zero\n");
    puts("other than zero");
    return 0;
}
