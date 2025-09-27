#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/jiffies.h>

//What is the problem with this code?

static int __init test_hello_init(void)
{
	unsigned long timeout = jiffies + 10 * HZ;

	pr_info("%s\n", __func__);
	if (timeout  > jiffies) {
		pr_warn("%s: no timeout\n", __func__);
	} else {
		pr_info("%s: timeout\n", __func__);
	}	

	return -1;
}

static void __exit test_hello_exit(void)
{
}

MODULE_LICENSE("GPL");

module_init(test_hello_init);
module_exit(test_hello_exit);
