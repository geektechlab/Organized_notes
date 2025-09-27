#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>

MODULE_LICENSE("GPL");

static struct task_struct* my_thread = NULL;

struct task_struct* thread_create1(void)
{
	return NULL;
}

struct task_struct* thread_create2(void)
{
	return ERR_PTR(-ENOMEM);
}

static int __init test_hello_init(void)
{
	pr_info("%s: In exit\n", __func__);
	my_thread = thread_create1();
	if (IS_ERR(my_thread)) {
		pr_err("kthread_create returned:%ld\n", PTR_ERR(my_thread));
	}

	my_thread = thread_create2();
	if (IS_ERR(my_thread)) {
		pr_err("kthread_create returned:%ld\n", PTR_ERR(my_thread));
	}
	return 0;
}

static void __exit test_hello_exit(void)
{

}

module_init(test_hello_init);
module_exit(test_hello_exit);
