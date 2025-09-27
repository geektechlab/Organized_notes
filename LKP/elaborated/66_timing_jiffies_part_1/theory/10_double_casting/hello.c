#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/jiffies.h>


static int __init test_hello_init(void)
{
	pr_info("jiffies:%lu\n", jiffies);
	pr_info("Initial value of jiffies:%lu\n", INITIAL_JIFFIES);
	pr_info("Diff:%lu\n", ((unsigned long)(unsigned int)(-1)) - INITIAL_JIFFIES);
	pr_info("Diff:%lu\n", ((unsigned long)(-1)) - INITIAL_JIFFIES);
	return 0;
}

static void __exit test_hello_exit(void)
{
}

MODULE_LICENSE("GPL");

module_init(test_hello_init);
module_exit(test_hello_exit);
