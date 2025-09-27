#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
static struct proc_dir_entry *embedded_pde; //pde -proc directory entry
static int test_proc_dir_init(void)
{
	pr_info("%s: In init\n", __func__);
	embedded_pde = proc_mkdir("embedded", NULL);
	if (!embedded_pde) {
		pr_err("%s: error creating proc_dir\n", __func__);
		return -1;
	}
	pr_info("Embedded Proc directory created\n");
	return 0;
}

static void test_proc_dir_exit(void)
{
	pr_info("%s: In exit\n", __func__);
	remove_proc_entry("embedded", NULL);
}

module_init(test_proc_dir_init);
module_exit(test_proc_dir_exit);
