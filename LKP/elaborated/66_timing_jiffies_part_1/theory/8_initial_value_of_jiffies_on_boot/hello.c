#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/jiffies.h>


static int __init test_hello_init(void)
{
	pr_info("jiffies:%lu\n", jiffies);
	return 0;
}

static void __exit test_hello_exit(void)
{
}

MODULE_LICENSE("GPL");

module_init(test_hello_init);
module_exit(test_hello_exit);
