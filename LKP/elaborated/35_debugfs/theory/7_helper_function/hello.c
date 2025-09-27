#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/debugfs.h>

MODULE_LICENSE("GPL");

static u8 value;
static struct dentry *dentry_u8;

static int __init test_hello_init(void)
{
	dentry_u8 = debugfs_create_u8("my_u8", 0666, NULL, &value);
	if (!dentry_u8) {
		printk("Error failed debugfs_create_u8");
		return -ENODEV;
	}

	return 0;
}

static void __exit test_hello_exit(void)
{
	debugfs_remove(dentry_u8);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
