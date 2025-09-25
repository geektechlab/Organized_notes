#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/hrtimer.h>

struct hrtimer timer;
static u64 period_ms = 250; // 250ms

enum hrtimer_restart my_hrtimer_function(struct hrtimer *timer)
{
	pr_info("timer fired:%lld\n", ktime_get());

	return HRTIMER_NORESTART;
}

static int test__init(void)
{
	pr_info("%s\n", __func__);
	hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	timer.function = my_hrtimer_function;

	pr_info("softexpires:%lld\n", timer._softexpires);
	hrtimer_start(&timer, ms_to_ktime(period_ms), HRTIMER_MODE_REL);
	pr_info("timer started:%lld\n", ktime_get());
	pr_info("softexpires after starting timer:%lld\n", timer._softexpires);

	return 0;
}

static void test__exit(void)
{
}

module_init(test__init);
module_exit(test__exit);

MODULE_LICENSE("GPL");
