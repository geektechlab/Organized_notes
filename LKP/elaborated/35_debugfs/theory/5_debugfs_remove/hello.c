#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/debugfs.h>

MODULE_LICENSE("GPL");
struct dentry *dbg_dir, *dbg_file;

static ssize_t dbg_read(struct file *file, char __user *buf,
                        size_t count, loff_t *ppos)
{
	pr_info("%s\n", __func__);
	return 0;
}

static ssize_t dbg_write(struct file *file, char const __user *buf,
                        size_t count, loff_t *ppos)
{
	pr_info("%s\n", __func__);
	return count;
}

static const struct file_operations dbg_fops = {
        .owner = THIS_MODULE,
        .read = dbg_read,
        .write = dbg_write
};

static int __init test_hello_init(void)
{
	pr_info("%s: In init\n", __func__);
	dbg_dir = debugfs_create_dir("my_dbg_dir", NULL);
	if (IS_ERR_OR_NULL(dbg_dir)) {
		pr_err("Failed to create debugfs directory\n");
		if (dbg_dir) {
			pr_err("Error code:%02lx\n", PTR_ERR(dbg_dir));
		}
		return -1;
	}
	pr_info("Debugfs directory created\n");
	dbg_file = debugfs_create_file("my_dbg_file", 0644, dbg_dir, NULL, &dbg_fops);
	if (IS_ERR_OR_NULL(dbg_file)) {
		pr_err("Failed to create debugfs file\n");
		if (dbg_file) {
			pr_err("Error code:%02lx\n", PTR_ERR(dbg_file));
		}
		debugfs_remove(dbg_dir);
		return -1;
	}
	pr_info("Debug file created\n");
	return 0;
}

static void __exit test_hello_exit(void)
{
	pr_info("%s: In exit\n", __func__);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
