#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

struct task_struct *thread;
wait_queue_head_t event_queue;
char buffer[256];

static int myfunc(void *data)
{
	pr_info("processor:%d\t%s:start()\n", smp_processor_id(), __func__);
	msleep(5000);
	pr_info("processor:%d\t%s:end()\n", smp_processor_id(), __func__);
	wake_up(&event_queue);
	return 0;
}

char * get_task_state(long state)
{
    switch (state) {
        case TASK_RUNNING:
            return "TASK_RUNNING";
        case TASK_INTERRUPTIBLE:
            return "TASK_INTERRUPTIBLE";
        case TASK_UNINTERRUPTIBLE:
            return "TASK_UNINTERRUPTIBLE";
        case __TASK_STOPPED:
            return "__TASK_STOPPED";
        case __TASK_TRACED:
            return "__TASK_TRACED";
        case TASK_IDLE:
            return "(TASK_UNINTERRUPTIBLE | TASK_NOLOAD)";
	case TASK_KILLABLE:
            return "(TASK_WAKEKILL | TASK_UNINTERRUPTIBLE)";
        case TASK_STOPPED:
            return "(TASK_WAKEKILL | __TASK_STOPPED)";
        case TASK_TRACED:
            return "(TASK_WAKEKILL | __TASK_TRACED)";
        default:
        {
            sprintf(buffer, "Unknown Type:%ld", state);
            return buffer;
        }
    }
}

static int __init test_hello_init(void)
{
    wait_queue_entry_t mywait;
    init_waitqueue_head(&event_queue);
    thread = kthread_run(myfunc, NULL, "wakeupthread");
    init_waitqueue_entry(&mywait, current);
    set_current_state(TASK_INTERRUPTIBLE);
    add_wait_queue(&event_queue, &mywait);
    pr_info("before calling schedule: state:%s\n", get_task_state(current->state));
    schedule();
    pr_info("after calling schedule: state:%s\n", get_task_state(current->state));
    remove_wait_queue(&event_queue, &mywait);
    return 0;
}

static void __exit test_hello_exit(void)
{
}

module_init(test_hello_init);
module_exit(test_hello_exit);
