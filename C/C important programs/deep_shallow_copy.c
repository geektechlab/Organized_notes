#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* str;
} Deep;

void deepCopy(Deep* dest, Deep* src) {
    dest->str = (char*)malloc(strlen(src->str) + 1);
    strcpy(dest->str, src->str);
}

int main() {
    Deep obj1;
    obj1.str = (char*)malloc(20 * sizeof(char));
    strcpy(obj1.str, "Hello");

    Deep obj2;
    deepCopy(&obj2, &obj1); // Deep copy

    printf("%s\n", obj2.str);

    // Free memory safely
    free(obj1.str);
    free(obj2.str);

    return 0;
}
