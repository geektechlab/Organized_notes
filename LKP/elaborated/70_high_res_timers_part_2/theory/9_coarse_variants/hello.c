#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ktime.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

u64 tsc_start;            
u64 tsc_end;              

static int test__init(void)
{
	ktime_t time;
	tsc_start = rdtsc_ordered();
	time = ktime_get_boottime();
	tsc_end = rdtsc_ordered();
	pr_info("non coarse boottime:%lld took %llu cycles\n",
			time, (tsc_end - tsc_start));
	tsc_start = rdtsc_ordered();
	time = ktime_get_coarse_boottime();
	tsc_end = rdtsc_ordered();
	pr_info(" coarse boottime:%lld took %llu cycles\n",
			time, (tsc_end - tsc_start));

	return 0;
}

static void test__exit(void)
{
        pr_info("%s: In exit\n", __func__);
}

module_init(test__init);
module_exit(test__exit);
