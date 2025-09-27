#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timex.h>
#include <linux/delay.h>

static int __init test_hello_init(void)
{
	u64 cycle1;
	u64 cycle2;

	cycle1 = get_cycles();
	ssleep(1);
	cycle2 = get_cycles();
	pr_info("diff:%llu cycles\n", cycle2 - cycle1);

	return 0;
}

static void __exit test_hello_exit(void)
{
}

MODULE_LICENSE("GPL");

module_init(test_hello_init);
module_exit(test_hello_exit);
