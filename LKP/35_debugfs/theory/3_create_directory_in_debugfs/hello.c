#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/debugfs.h>

MODULE_LICENSE("GPL");
struct dentry *dbg_dir;

static int __init test_hello_init(void)
{
	pr_info("%s: In init\n", __func__);
	dbg_dir = debugfs_create_dir("my_dbg_dir", NULL);
	if (IS_ERR_OR_NULL(dbg_dir)) {
		pr_err("Failed to create debugfs directoryn");
		if (dbg_dir) {
			pr_err("Error code:%02lx\n", PTR_ERR(dbg_dir));
		}
		return -1;
	}
	pr_info("Debugfs directory created\n");
	return 0;
}

static void __exit test_hello_exit(void)
{
	pr_info("%s: In exit\n", __func__);
	debugfs_remove(dbg_dir);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
