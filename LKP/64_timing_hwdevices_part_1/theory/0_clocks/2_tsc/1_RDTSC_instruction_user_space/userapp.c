#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main() 
{
	uint64_t val1, val2;
	unsigned int h, l;
	__asm__ __volatile__("rdtsc" : "=a" (l), "=d" (h));
	val1 = ((uint64_t)l) | (((uint64_t)h) << 32);
	sleep(1);
	__asm__ __volatile__("rdtsc" : "=a" (l), "=d" (h));
	val2 = ((uint64_t)l) | (((uint64_t)h) << 32);

	printf("%llu \n", val2 - val1);
	return 0;
}
