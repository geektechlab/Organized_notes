#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/jiffies.h>

static int __init test_hello_init(void)
{
	unsigned long timeout; 
	pr_info("starting busy loop\n");
	timeout = jiffies + 2 * HZ; //2 Sec
	while(time_before(jiffies, timeout));
	pr_info("ended busy loop\n");

	return 0;
}

static void __exit test_hello_exit(void)
{
}

MODULE_LICENSE("GPL");

module_init(test_hello_init);
module_exit(test_hello_exit);
