#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/hrtimer.h>

/* fire timer after 1 sec */

/* this also gives CPU frequemcy */

struct hrtimer timer;
u64 tsc_start;            /* TSC when timer was queued */
u64 tsc_end;              /* TSC when timer is executed */

enum hrtimer_restart my_hrtimer_function(struct hrtimer *timer)
{
	tsc_end = rdtsc_ordered();
	pr_info("%s\n", __func__);
	pr_info("Diff:%lld\n", (tsc_end - tsc_start));

	return HRTIMER_NORESTART;
}

static int test__init(void)
{
	ktime_t time;
	pr_info("%s\n", __func__);
	hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	timer.function = my_hrtimer_function;

	time = ktime_set(1, 0);
	pr_info("time:%lld\n", time);
	tsc_start = rdtsc_ordered();
	hrtimer_start(&timer, time, HRTIMER_MODE_REL);

	return 0;
}

static void test__exit(void)
{
}

module_init(test__init);
module_exit(test__exit);

MODULE_LICENSE("GPL");
