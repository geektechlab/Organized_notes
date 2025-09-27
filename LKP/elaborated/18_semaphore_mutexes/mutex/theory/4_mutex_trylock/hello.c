#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

DEFINE_MUTEX(mylock);
static struct task_struct *thread1, *thread2;

static int threadfn(void *arg)
{
	int ret;
	ret = mutex_trylock(&mylock);
	pr_info("Starting critical region on processor:%d with ret:%d\n", smp_processor_id(), ret);
	msleep(2000);
	pr_info("Ending critical region\n");
	if (!ret)
		mutex_unlock(&mylock);
	return 0;
}

static int __init test_hello_init(void)
{
    mutex_init(&mylock);
    thread1 = kthread_run(threadfn, NULL, "thread1");
    thread2 = kthread_run(threadfn, NULL, "thread2");

    return 0;
}

static void __exit test_hello_exit(void)
{
}

module_init(test_hello_init);
module_exit(test_hello_exit);
