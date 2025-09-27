#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/jiffies.h>

static int __init test_hello_init(void)
{
	unsigned long timeout1 = jiffies + 10 * HZ;
	unsigned long timeout2 = jiffies +  5 * HZ;

	pr_info("%s timeout1:%lu\t timeout2:%lu\n", __func__, timeout1, timeout2);
	pr_info("%s: timeout1 - timeout2:%u\n", __func__, ((long)(timeout1 - timeout2) < 0));
	pr_info("%s: timeout2 - timeout1:%u\n", __func__, ((long)(timeout2 - timeout1) < 0));

	return -1;
}

static void __exit test_hello_exit(void)
{
}

MODULE_LICENSE("GPL");

module_init(test_hello_init);
module_exit(test_hello_exit);
