#include <stdio.h>
#include <time.h>

int main(int argc, char **argv)
{
    struct timespec res;
    clockid_t clk_id;

    clk_id = CLOCK_REALTIME;
    clock_getres(clk_id, &res);
    printf("res.tv_sec: %ld\n", res.tv_sec);
    printf("res.tv_nsec: %ld\n", res.tv_nsec);

    clk_id = CLOCK_MONOTONIC;
    clock_getres(clk_id, &res);
    printf("res.tv_sec: %ld\n", res.tv_sec);
    printf("res.tv_nsec: %ld\n", res.tv_nsec);

    clk_id = CLOCK_BOOTTIME;
    clock_getres(clk_id, &res);
    printf("res.tv_sec: %ld\n", res.tv_sec);
    printf("res.tv_nsec: %ld\n", res.tv_nsec);

    clk_id = CLOCK_PROCESS_CPUTIME_ID;
    clock_getres(clk_id, &res);
    printf("res.tv_sec: %ld\n", res.tv_sec);
    printf("res.tv_nsec: %ld\n", res.tv_nsec);

    clk_id = CLOCK_THREAD_CPUTIME_ID;
    clock_getres(clk_id, &res);
    printf("res.tv_sec: %ld\n", res.tv_sec);
    printf("res.tv_nsec: %ld\n", res.tv_nsec);

    return 0;
}
