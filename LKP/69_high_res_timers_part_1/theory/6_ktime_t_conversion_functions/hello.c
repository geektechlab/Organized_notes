#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ktime.h>

MODULE_LICENSE("GPL");


static int test__init(void)
{
	ktime_t ktime;
	struct timeval time_val;
	struct timespec time_spec;

	ktime = ktime_set(100, 10);
	pr_info("ktime:%lld\n", ktime);
	
	time_val = ktime_to_timeval(ktime);
	time_spec = ktime_to_timespec(ktime);

	pr_info("timeval sec:%ld\t usec:%ld\n", time_val.tv_sec, time_val.tv_usec);
	pr_info("timespec sec:%ld\t nsec:%ld\n", time_spec.tv_sec, time_spec.tv_nsec);
	return 0;
}

static void test__exit(void)
{
        pr_info("%s: In exit\n", __func__);
}

module_init(test__init);
module_exit(test__exit);
