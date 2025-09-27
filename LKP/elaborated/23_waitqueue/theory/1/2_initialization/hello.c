#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");


static int __init test_hello_init(void)
{
    DEFINE_WAIT(mywait);
    pr_info("flags:%02x\n", mywait.flags);
    pr_info("comm:%s\n",((struct task_struct *)(mywait.private))->comm);
    return 0;
}

static void __exit test_hello_exit(void)
{
}

module_init(test_hello_init);
module_exit(test_hello_exit);
