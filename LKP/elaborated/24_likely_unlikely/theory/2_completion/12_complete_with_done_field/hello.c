#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/completion.h>
#include <linux/delay.h>

DECLARE_COMPLETION(my_completion);

static struct task_struct *kthread1, *kthread2, *kthread3, *kthread4;

int condition = 0;

MODULE_LICENSE("GPL");

static int threadfn(void *unused)
{
	pr_info("%s:Processor:%d starting done:%u\n", __func__, smp_processor_id(), my_completion.done);
	pr_info("%s:Processor:%d calling wait_for_completion:%u\n", __func__, smp_processor_id(), my_completion.done);
	wait_for_completion(&my_completion);
	pr_info("%s:Processor:%d after wait_for_completion:%u\n", __func__, smp_processor_id(), my_completion.done);
	pr_info("%s:Processor:%d ending\n", __func__, smp_processor_id());
	return 0;
}

static int mod_init(void)
{
	pr_info("%s Processor:%d starting completion test Done:%u\n", __func__, smp_processor_id(),
			my_completion.done);
	kthread1 = kthread_run(threadfn, NULL, "mythread1");
	kthread2 = kthread_run(threadfn, NULL, "mythread2");
	kthread3 = kthread_run(threadfn, NULL, "mythread3");
	kthread4 = kthread_run(threadfn, NULL, "mythread4");
	mdelay(5000);
	pr_info("%s done:%u before calling complete first time\n", __func__, my_completion.done);
	complete(&my_completion);
	pr_info("%s done:%u after calling complete first time\n", __func__, my_completion.done);
	complete(&my_completion);
	pr_info("%s done:%u after calling complete second time\n", __func__, my_completion.done);
	complete(&my_completion);
	pr_info("%s done:%u after calling complete third time\n", __func__, my_completion.done);
	complete(&my_completion);
	pr_info("%s done:%u after calling complete fourth time\n", __func__, my_completion.done);
	return 0;
}

static void mod_exit(void)
{
	pr_info("%s\n", __func__);
}

module_init(mod_init);
module_exit(mod_exit);
