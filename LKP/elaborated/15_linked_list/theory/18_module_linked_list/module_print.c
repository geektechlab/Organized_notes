#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");


static int module_print_init(void)
{
	struct module *m = THIS_MODULE;
	struct list_head *modules;
	int j = 0;

	modules = &m->list;
	modules = modules->prev;

	list_for_each_entry(m, modules, list) {
		pr_info("%3d module name:%20s, symbol exported= %u\n", j++, m->name, m->num_syms);
	}
	return -1;
}

static void module_print_exit(void)
{
}

module_init(module_print_init);
module_exit(module_print_exit);


