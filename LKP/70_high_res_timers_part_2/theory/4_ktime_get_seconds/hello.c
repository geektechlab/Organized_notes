#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ktime.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

time64_t module_start_seconds, module_end_seconds;

static int test__init(void)
{
	module_start_seconds = ktime_get_seconds();
	pr_info("seconds:%lld\n", module_start_seconds);
	return 0;
}

static void test__exit(void)
{
	module_end_seconds = ktime_get_seconds();
	pr_info("seconds:%lld\n", module_end_seconds);
        pr_info("%s: In exit Module was loaded for %lld seconds\n", __func__,
		module_end_seconds - module_start_seconds);
}

module_init(test__init);
module_exit(test__exit);
