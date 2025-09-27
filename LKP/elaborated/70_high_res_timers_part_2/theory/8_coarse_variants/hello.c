#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ktime.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

static int test__init(void)
{
	pr_info("RealTime  (non coarse):%lld\t (coarse):%lld\n", ktime_get_real(), ktime_get_coarse_real());
	pr_info("BootTime  (non coarse):%lld\t (coarse):%lld\n", ktime_get_boottime(), ktime_get_coarse_boottime());
	return 0;
}

static void test__exit(void)
{
        pr_info("%s: In exit\n", __func__);
}

module_init(test__init);
module_exit(test__exit);
