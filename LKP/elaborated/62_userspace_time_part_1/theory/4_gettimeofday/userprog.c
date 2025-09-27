#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char argv[])
{
    struct timeval tv;
    unsigned long long milliseconds = 0;

    gettimeofday(&tv, NULL);
    milliseconds = (tv.tv_sec * 1000) + (tv.tv_usec/1000);
    printf("Time:%s", ctime(&tv.tv_sec));
    printf("Number of seconds since epoch time:%lu\n", tv.tv_sec);
    printf("Number of milliseconds since epoch time :%llu\n", milliseconds);

    return 0;
}

