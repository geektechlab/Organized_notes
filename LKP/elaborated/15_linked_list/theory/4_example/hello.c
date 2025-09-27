#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");

LIST_HEAD(listhead);

static int __init test_hello_init(void)
{
    pr_info("%s: In init\n", __func__);
    pr_info("%s: listhead:%px\t listhead.next:%px\t listhead.prev:%px\n",
		    __func__,&listhead, listhead.next, listhead.prev);
    return -1;
}

static void __exit test_hello_exit(void)
{
    pr_info("%s: In exit\n", __func__);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
