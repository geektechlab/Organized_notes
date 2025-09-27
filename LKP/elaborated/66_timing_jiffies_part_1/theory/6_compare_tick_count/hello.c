#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/jiffies.h>

static int __init test_hello_init(void)
{
	unsigned long timeout = jiffies + 10 * HZ;

	pr_info("%s jiffies:%lu\t timeout:%lu\n", __func__, jiffies, timeout);
	pr_info("%s: time_after (timeout, jiffies):%d\n", __func__, time_after(timeout, jiffies));
	pr_info("%s: time_after_eq (timeout, jiffies):%d\n", __func__, time_after_eq(timeout, jiffies));
	pr_info("%s: time_before (timeout, jiffies):%d\n", __func__, time_before(timeout, jiffies));
	pr_info("%s: time_before_eq (timeout, jiffies):%d\n", __func__, time_before_eq(timeout, jiffies));
	if (time_after(timeout, jiffies)) {
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
