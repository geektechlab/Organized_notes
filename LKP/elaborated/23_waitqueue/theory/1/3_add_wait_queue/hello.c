#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

struct task_struct *thread1, *thread2;
wait_queue_head_t event_queue;
char buffer[256];

static int myfunc(void *data)
{
	pr_info("processor:%d\t%s:start()\n", smp_processor_id(), __func__);
	pr_info("processor:%d\t pid:%d\n", smp_processor_id(), current->pid);
	pr_info("processor:%d\t%s:end()\n", smp_processor_id(), __func__);
	return 0;
}


static int __init test_hello_init(void)
{
    wait_queue_entry_t mywait1, mywait2, *curr, *next;
    init_waitqueue_head(&event_queue);

    thread1 = kthread_run(myfunc, NULL, "thread1");
    thread2 = kthread_run(myfunc, NULL, "thread2");

    init_waitqueue_entry(&mywait1, thread1);
    init_waitqueue_entry(&mywait2, thread2);

    add_wait_queue(&event_queue, &mywait1);
    add_wait_queue_exclusive(&event_queue, &mywait2);

    list_for_each_entry_safe(curr, next, &(event_queue.head), entry){
	    pr_info("flag:%d\n", curr->flags);
	    pr_info("pid:%d\n", ((struct task_struct *)(curr->private))->pid);
    }
    pr_info("the current pid is :%d\n",current->pid);
    return 0;
}

static void __exit test_hello_exit(void)
{
}

module_init(test_hello_init);
module_exit(test_hello_exit);
