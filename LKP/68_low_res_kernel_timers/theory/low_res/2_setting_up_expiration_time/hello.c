#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>

static void timer_func(struct timer_list *unused)
{
	pr_info("%s jiffies:%lu\n", __func__, jiffies);
}

static DEFINE_TIMER(my_timer, timer_func);

static int test__init(void)
{
	pr_info("%s\n", __func__);
        pr_info("expires:%lu\n", my_timer.expires);	
        mod_timer(&my_timer, jiffies+1000);
	pr_info("expires:%lu\n", my_timer.expires);
	return 0;
}

static void test__exit(void)
{
}

module_init(test__init);
module_exit(test__exit);

MODULE_LICENSE("GPL");
