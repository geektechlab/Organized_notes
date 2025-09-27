#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ktime.h>

MODULE_LICENSE("GPL");


static int test__init(void)
{
	ktime_t time1, time2;
	struct timespec time_spec;
	time_spec.tv_sec = 100;
	time_spec.tv_nsec = 11;;
	
	time1 = ktime_set(100, 10);
	pr_info("time1:%lld\n", time1);
	
	time2 = timespec_to_ktime(time_spec);
	pr_info("time2:%lld\n", time2);

	pr_info("Diff:%lld\n", ktime_sub(time2, time1));
	return 0;
}

static void test__exit(void)
{
        pr_info("%s: In exit\n", __func__);
}

module_init(test__init);
module_exit(test__exit);
