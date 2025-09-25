#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ktime.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

static int test__init(void)
{
	time64_t seconds;

	seconds = ktime_get_seconds();
	pr_info("seconds:%lld\n", seconds);

	seconds = ktime_get_boottime_seconds();
	pr_info("boot time seconds:%lld\n", seconds);

	seconds = ktime_get_real_seconds();
	pr_info(" real time seconds:%lld\n", seconds);

	return 0;
}

static void test__exit(void)
{
        pr_info("%s: In exit\n", __func__);
}

module_init(test__init);
module_exit(test__exit);
