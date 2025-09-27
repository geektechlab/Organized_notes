#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ktime.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

static int test__init(void)
{
	struct timespec64 time_spec;
	ktime_get_real_ts64(&time_spec);
	pr_info(" Real time spec seconds:%lld\t nanoseconds:%ld\n", 
		time_spec.tv_sec, time_spec.tv_nsec);
	ktime_get_boottime_ts64(&time_spec);
	pr_info(" Boot time spec seconds:%lld\t nanoseconds:%ld\n", 
		time_spec.tv_sec, time_spec.tv_nsec);
	return 0;
}

static void test__exit(void)
{
        pr_info("%s: In exit\n", __func__);
}

module_init(test__init);
module_exit(test__exit);
