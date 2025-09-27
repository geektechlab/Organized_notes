#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ktime.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

static int test__init(void)
{
	u64 time1, time2;
	time1 = ktime_get_real_ns();
	mdelay(1000);
	time2 = ktime_get_real_ns();
	pr_info("time1:%lld\t time2:%lld\n", time1, time2);
	pr_info("diff:%lld\n", time2 - time1);
	return 0;
}

static void test__exit(void)
{
        pr_info("%s: In exit\n", __func__);
}

module_init(test__init);
module_exit(test__exit);
