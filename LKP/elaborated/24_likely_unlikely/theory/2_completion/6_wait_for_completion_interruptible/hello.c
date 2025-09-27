#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/completion.h>
#include <linux/delay.h>

DECLARE_COMPLETION(my_completion);

static struct task_struct *kthread = NULL;

int condition = 0;

MODULE_LICENSE("GPL");

static int threadfn(void *unused)
{
	pr_info("%s:Processor:%d starting done:%d\n", __func__, smp_processor_id(), my_completion.done);
	mdelay(10000);
	pr_info("%s:Processor:%d calling complete:%d\n", __func__, smp_processor_id(), my_completion.done);
	complete(&my_completion);
	pr_info("%s:Processor:%d after complete:%d\n", __func__, smp_processor_id(), my_completion.done);
	pr_info("%s:Processor:%d ending\n", __func__, smp_processor_id());
	return 0;
}

static int mod_init(void)
{
	int ret;
	pr_info("%s Processor:%d starting completion test Done:%d\n", __func__, smp_processor_id(),
			my_completion.done);
	kthread = kthread_run(threadfn, NULL, "mythread");
	pr_info("%s Processor:%d waiting for completion:%d\n", __func__, smp_processor_id(),
			my_completion.done);
	ret = wait_for_completion_interruptible(&my_completion);
	pr_info("%s complete:%d ret:%d\n", __func__, my_completion.done, ret);
	return 0;
}

static void mod_exit(void)
{
	pr_info("%s\n", __func__);
}

module_init(mod_init);
module_exit(mod_exit);
