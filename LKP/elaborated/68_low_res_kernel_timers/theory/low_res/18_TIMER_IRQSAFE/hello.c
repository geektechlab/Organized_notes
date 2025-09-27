#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>

void is_irq_disabled(void)
{
        if (irqs_disabled())
                pr_info("IRQ Disabled\n");
        else
                pr_info("IRQ Enabled\n");
}

static void timer_func(struct timer_list *my_timer)
{
	pr_info("%s\n", __func__);
	is_irq_disabled();
}

struct timer_list my_timer;
struct timespec val;

static int test__init(void)
{
	pr_info("%s\n", __func__);
	timer_setup(&my_timer, timer_func, TIMER_IRQSAFE);
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
