#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/completion.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

DECLARE_COMPLETION(my_comp1);
struct completion *my_comp2;

static int __init test_hello_init(void)
{
    pr_info("my_comp1.done:%d\n", my_comp1.done);
    my_comp2 = kmalloc(sizeof(struct completion), GFP_KERNEL);
    pr_info("my_comp2.done before initialization:%d\n", my_comp2->done);
    init_completion(my_comp2);
    pr_info("my_comp2.done after initialization:%d\n", my_comp2->done);
    kfree(my_comp2);
    return 0;
}

static void __exit test_hello_exit(void)
{
}

module_init(test_hello_init);
module_exit(test_hello_exit);
