#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>

static void timer_func(struct timer_list *my_timer)
{
	pr_info("%s\n", __func__);
	pr_info("expires before mod_timer:%lu jiffies:%lu\n", 
		my_timer->expires, jiffies);
        pr_info("timer_pending returned:%d\n", timer_pending(my_timer));
	mod_timer(my_timer, jiffies + msecs_to_jiffies(1000));	
	pr_info("expires after mod_timer:%lu\n", my_timer->expires);
        pr_info("timer_pending returned:%d\n", timer_pending(my_timer));
}

static DEFINE_TIMER(my_timer, timer_func);
struct timespec val;

static int test__init(void)
{
	pr_info("%s\n", __func__);
	val.tv_sec = 10;
	val.tv_nsec = 0;
        mod_timer(&my_timer, jiffies + timespec_to_jiffies(&val));	
	return 0;
}

static void test__exit(void)
{
	pr_info("del_timer_sync returned:%d\n", del_timer_sync(&my_timer));
}

module_init(test__init);
module_exit(test__exit);

MODULE_LICENSE("GPL");
