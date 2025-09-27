#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* prints all environemnet variables */

extern char **environ;

int main()
{
    int index = 0;
    while (environ[index] != NULL) {
        printf("%s\n", environ[index]);
        index++;
    }
    return 0;
}
