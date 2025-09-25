#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

static inline uint64_t _rdtscp(void)
{
    unsigned long _hi, _lo;
    asm volatile("rdtscp" : "=a"(_lo), "=d"(_hi));
    uint64_t val = (((uint64_t)_hi) << 32 | ((uint64_t)_lo));
    return val;
}

int main() 
{
	uint64_t val1, val2;
	unsigned int h, l;
	val1 = _rdtscp();
	sleep(1);
	val2 = _rdtscp();
	printf("cycles:%llu \n", val2 - val1);

	return 0;
}
