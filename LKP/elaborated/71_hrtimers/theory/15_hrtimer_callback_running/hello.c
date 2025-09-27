#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/hrtimer.h>

struct hrtimer timer;

ktime_t time;

enum hrtimer_restart my_hrtimer_function(struct hrtimer *timer)
{
	pr_info("%s\n", __func__);
	pr_info("%s: hrtimer_callback_running:%d\n", __func__, hrtimer_callback_running(timer));
	return HRTIMER_NORESTART;
}

static int test__init(void)
{
	pr_info("%s\n", __func__);
	//CLOCK_MONOTONIC a clock which is guaranteed always to move forward in time
	hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	timer.function = my_hrtimer_function;

	//Expire time: 1sec, 0 nanosecond
	time = ktime_set(1, 0);
	pr_info("time:%lld\n", time);
	hrtimer_start(&timer, time, HRTIMER_MODE_REL);
	pr_info("%s: hrtimer_callback_running:%d\n", __func__, hrtimer_callback_running(&timer));

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
