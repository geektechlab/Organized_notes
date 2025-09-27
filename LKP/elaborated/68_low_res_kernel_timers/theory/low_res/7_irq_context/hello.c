#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/interrupt.h>

/* code is not running in hard irq context */

void print_context(void)
{
        if (in_irq()) {
                pr_info("Code is running in hard irq context\n");
        } else {
                pr_info("Code is not running in hard irq context\n");
        }
}

static void timer_func(struct timer_list *unused)
{
	pr_info("%s\n", __func__);
	print_context();
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
}

module_init(test__init);
module_exit(test__exit);

MODULE_LICENSE("GPL");
