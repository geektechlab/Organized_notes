#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

LIST_HEAD(listhead);

struct mylist
{
	int data;
	struct list_head list;
};

void add_data(int data)
{
	struct mylist *node = kmalloc(sizeof(struct mylist), GFP_KERNEL);
	pr_info("%s: Node:%px\t node.list:%px\n", __func__, node, &(node->list));
	if (node)
	{
		node->data = data;
		list_add(&node->list, &listhead);
	}
}

void add_data_end(int data)
{
	struct mylist *node = kmalloc(sizeof(struct mylist), GFP_KERNEL);
	pr_info("%s: Node:%px\t node.list:%px\n", __func__, node, &(node->list));
	if (node)
	{
		node->data = data;
		list_add_tail(&node->list, &listhead);
	}
}

static int __init test_hello_init(void)
{
    struct mylist *node;
    pr_info("%s: In init\n", __func__);
    pr_info("%s: listhead:%px\t listhead.next:%px\t listhead.prev:%px\n",
		    __func__,&listhead, listhead.next, listhead.prev);
    add_data_end(1);
    node = list_entry(listhead.next, struct mylist, list);
    pr_info("%s: node:%px data:%d\n", __func__, node, node->data); 

    return -1;
}

static void __exit test_hello_exit(void)
{
    pr_info("%s: In exit\n", __func__);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
