#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/delay.h>

u64 module_start_time, module_end_time;
static int __init test_hello_init(void)
{
	module_start_time = jiffies;
	pr_info("%s: In init\n", __func__);
	return 0;
}

static void __exit test_hello_exit(void)
{
	pr_info("%s: In exit\n", __func__);
	module_end_time = jiffies;
	pr_info("%s:Module Time:%llu seconds\n", __func__, (module_end_time - module_start_time)/HZ);
}

MODULE_LICENSE("GPL");

module_init(test_hello_init);
module_exit(test_hello_exit);
