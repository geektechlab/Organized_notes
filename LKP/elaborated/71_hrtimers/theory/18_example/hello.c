#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/hrtimer.h>

struct hrtimer timer;

ktime_t time;
ktime_t remaining;

enum hrtimer_restart my_hrtimer_function(struct hrtimer *timer)
{
	pr_info("%s\n", __func__);
	pr_info("%s: hrtimer_callback_running:%d\n", __func__, hrtimer_callback_running(timer));
	remaining = hrtimer_get_remaining(timer);
	pr_info("%s: remaining ktime:%lld\n", __func__, remaining);

	return HRTIMER_NORESTART;
}

static int test__init(void)
{
	pr_info("%s\n", __func__);
	hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_ABS);
	timer.function = my_hrtimer_function;

	time = ktime_set(1, 10);
	pr_info("time:%lld\n", time);
	hrtimer_start(&timer, time, HRTIMER_MODE_ABS);
	pr_info("%s: hrtimer_callback_running:%d\n", __func__, hrtimer_callback_running(&timer));
	remaining = hrtimer_get_remaining(&timer);
	pr_info("%s: remaining ktime:%lld\n", __func__, remaining);

	return 0;
}

static void test__exit(void)
{
	pr_info("%s\n", __func__);
	hrtimer_cancel(&timer);
}

module_init(test__init);
module_exit(test__exit);

MODULE_LICENSE("GPL");
