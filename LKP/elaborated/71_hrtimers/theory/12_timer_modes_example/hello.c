#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/hrtimer.h>

struct hrtimer timer1, timer2, timer3, timer4;
ktime_t time;

static int test__init(void)
{
	pr_info("%s\n", __func__);
	hrtimer_init(&timer1, CLOCK_REALTIME, HRTIMER_MODE_ABS);
	hrtimer_init(&timer2, CLOCK_MONOTONIC, HRTIMER_MODE_ABS);
	hrtimer_init(&timer3, CLOCK_REALTIME, HRTIMER_MODE_REL);
	hrtimer_init(&timer4, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	
	pr_info("CLOCK_REALTIME(absolute) gettime:%lld\n", timer1.base->get_time());
	pr_info("CLOCK_MONOTONIC(absolute) gettime:%lld\n", timer2.base->get_time());
	pr_info("CLOCK_REALTIME(relative) gettime:%lld\n", timer3.base->get_time());
	pr_info("CLOCK_MONOTONIC(relative) gettime:%lld\n", timer4.base->get_time());

	return 0;
}

static void test__exit(void)
{
}

module_init(test__init);
module_exit(test__exit);

MODULE_LICENSE("GPL");
