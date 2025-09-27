#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ktime.h>

MODULE_LICENSE("GPL");


static int test__init(void)
{
	ktime_t time1, time2;
	
	time1 = ktime_set(100, 10);
	pr_info("time1:%lld\n", time1);
	
	time2 = ktime_set(100, 11);
	pr_info("time2:%lld\n", time2);

	pr_info("Diff:%lld\n", time2-time1);
	return 0;
}

static void test__exit(void)
{
        pr_info("%s: In exit\n", __func__);
}

module_init(test__init);
module_exit(test__exit);
