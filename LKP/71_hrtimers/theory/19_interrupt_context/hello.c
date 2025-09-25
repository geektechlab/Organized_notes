#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/hrtimer.h>

/* runs in interrupt context */

struct hrtimer timer;

ktime_t time;

void print_context(void)
{
        if (in_interrupt()) {
                pr_info("Code is running in interrupt context\n");
        } else {
                pr_info("Code is running in process context\n");
        }
}

enum hrtimer_restart my_hrtimer_function(struct hrtimer *timer)
{
	pr_info("%s\n", __func__);
	print_context();

	return HRTIMER_NORESTART;
}

static int test__init(void)
{
	pr_info("%s\n", __func__);

	hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	timer.function = my_hrtimer_function;

	//Expire time: 1sec, 10 nanosecond
	time = ktime_set(1, 10);
	pr_info("time:%lld\n", time);
	hrtimer_start(&timer, time, HRTIMER_MODE_REL);

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
