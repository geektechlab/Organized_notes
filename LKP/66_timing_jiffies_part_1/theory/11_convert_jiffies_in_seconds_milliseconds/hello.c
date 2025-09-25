#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/jiffies.h>

static int __init test_hello_init(void)
{
	unsigned long current_jiffie = jiffies;
	pr_info("jiffie: %lu\n", current_jiffie);
	pr_info("jiffies_to_msecs:%lu\n", jiffies_to_msecs(current_jiffie));
	pr_info("jiffies_to_usecs:%lu\n", jiffies_to_usecs(current_jiffie));
	pr_info("jiffies_to_nsecs:%llu\n", jiffies_to_nsecs(current_jiffie));
	return 0;
}

static void __exit test_hello_exit(void)
{
}

MODULE_LICENSE("GPL");

module_init(test_hello_init);
module_exit(test_hello_exit);
