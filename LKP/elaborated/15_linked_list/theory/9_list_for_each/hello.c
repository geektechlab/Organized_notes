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

void traversal(void)
{
	struct list_head *ptr;
	struct mylist *node;
	for(ptr = listhead.next; ptr != &listhead; ptr = ptr->next)
	{
		node = list_entry(ptr, struct mylist, list);
		pr_info("%s: Data:%d\n", __func__, node->data);
	}
}

void traversal_ll(void)
{
	struct list_head *ptr;
	struct mylist *node;
	list_for_each(ptr, &listhead)
	{	
		node = list_entry(ptr, struct mylist, list);
		pr_info("%s: Data:%d\n", __func__, node->data);
	}

}

static int __init test_hello_init(void)
{
    add_data_end(1);
    add_data_end(2);

    traversal_ll();
    return -1;
}

static void __exit test_hello_exit(void)
{
    pr_info("%s: In exit\n", __func__);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
