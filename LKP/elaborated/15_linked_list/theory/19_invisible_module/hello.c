#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static struct list_head *module_previous; // Used to store the previous module when hiding this module from the list of all modules
struct task_struct *print_thread;

void hide_module(void) {
	pr_info("Hiding Module\n");
	module_previous = THIS_MODULE->list.prev;
	list_del(&THIS_MODULE->list);
}

void unhide_module(void) {
	pr_info("Unhiding module\n");
	list_add(&THIS_MODULE->list, module_previous);
}

static int hiding_thread(void *data)
{
	while(!kthread_should_stop()) {
		hide_module();
		msleep(5000);
		unhide_module();
		msleep(5000);
	}
	return 0;
}

MODULE_LICENSE("GPL");
static int test_tasks_init(void)
{
	pr_info("%s: In init\n", __func__);
	print_thread = kthread_run(hiding_thread, NULL, "hide_module");
	return 0;
}

static void test_tasks_exit(void)
{
	pr_info("%s: In exit\n", __func__);
	kthread_stop(print_thread);
}

module_init(test_tasks_init);
module_exit(test_tasks_exit);
