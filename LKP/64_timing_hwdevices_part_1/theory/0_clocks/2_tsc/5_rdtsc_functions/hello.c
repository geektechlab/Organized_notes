#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/msr.h>

static int __init test_hello_init(void)
{
	u64 tsc = rdtsc();
	u64 ordered_tsc = rdtsc_ordered();
	pr_info("tsc:%llu\n", tsc);
	pr_info("ordered_tsc:%llu\n", ordered_tsc);

	return 0;
}

static void __exit test_hello_exit(void)
{
}

MODULE_LICENSE("GPL");

module_init(test_hello_init);
module_exit(test_hello_exit);
