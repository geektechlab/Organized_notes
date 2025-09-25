#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ktime.h>

MODULE_LICENSE("GPL");

static int test__init(void)
{
	ktime_t ktime;

	ktime = ktime_set(100, 10);
	pr_info("ktime:%lld\n", ktime);
	pr_info("nanoseconds:%lld\n", ktime_to_ns(ktime));	
	return 0;
}

static void test__exit(void)
{
        pr_info("%s: In exit\n", __func__);
}

module_init(test__init);
module_exit(test__exit);
