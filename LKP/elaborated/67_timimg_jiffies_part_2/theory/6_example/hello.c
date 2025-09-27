#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/jiffies.h>

static unsigned long prev, now;
static int __init test_hello_init(void)
{
	unsigned long timeout;

	pr_info("%s: In init\n", __func__);
	prev = jiffies;
	printk("HZ=%d\n", HZ);
	timeout = jiffies + 5*HZ;
	printk("%s jiffies=%lu\t timeout:%lu\n", __func__, prev, timeout);
	while (time_before(jiffies, timeout)) continue;
	printk("after while loop: jiffies:%lu\n", jiffies);

	now = jiffies;
	printk("time_after=%d\n", time_after(now, prev));
        printk("time_before=%d\n", time_before(now, prev));
        printk("time_after_eq=%d\n", time_after_eq(now, prev));
        printk("time_before_eq=%d\n", time_before_eq(now, prev));

	return -1;
}

static void __exit test_hello_exit(void)
{
	pr_info("%s: In exit\n", __func__);
}

MODULE_LICENSE("GPL");

module_init(test_hello_init);
module_exit(test_hello_exit);
