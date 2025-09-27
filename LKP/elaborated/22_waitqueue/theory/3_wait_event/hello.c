#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/wait.h>

wait_queue_head_t event_queue;

static struct task_struct *kthread = NULL;

int condition = 0;

MODULE_LICENSE("GPL");

static int threadfn(void *unused)
{
	pr_info("%s: starting\n", __func__);
	condition = 1;
	wake_up(&event_queue);
	pr_info("%s: ending\n", __func__);
	return 0;
}

static int mod_init(void)
{
	pr_info("%s\n", __func__);
	init_waitqueue_head(&event_queue);

	kthread = kthread_run(threadfn, NULL, "mythread");
	pr_info("%s Waiting for thread to complete\n", __func__);
	wait_event(event_queue, condition);
	pr_info("%s complete\n", __func__);
	return 0;
}

static void mod_exit(void)
{
	pr_info("%s\n", __func__);
}

module_init(mod_init);
module_exit(mod_exit);

