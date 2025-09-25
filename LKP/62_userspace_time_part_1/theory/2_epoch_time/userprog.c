#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char argv[])
{
    printf("Epoch Time %ld\n", time(NULL));
    sleep(5);
    printf("Epoch Time %ld\n", time(NULL));
    return 0;
}

