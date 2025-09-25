#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

// get time rtdsc
unsigned long long int get_time(void)
{
    unsigned long int lo, hi;
    asm( "rdtsc" : "=a" (lo), "=d" (hi) );
    return( lo | ((uint64_t)hi << 32) );
}

unsigned long long int start, end;

static int test__init(void)
{
    pr_info("%s: In init processorid:%d\n", __func__, smp_processor_id());
	start = get_time();
	ssleep(1);
	end = get_time();
	pr_info("cycles:%lld\n", end-start);

	return 0;
}

static void test__exit(void)
{
    pr_info("%s: In exit\n", __func__);
}

module_init(test__init);
module_exit(test__exit);
