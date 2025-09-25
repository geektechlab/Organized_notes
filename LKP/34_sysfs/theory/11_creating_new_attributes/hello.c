#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

MODULE_LICENSE("GPL");
static struct kobj_attribute my_attr;

static ssize_t my_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf)
{
        pr_info("%s\n", __func__);
        return strlen(buf);
}

static ssize_t my_store(struct  kobject *kobj, struct kobj_attribute *attr,
                        const char *buf,size_t count)
{
        pr_info("%s\n", __func__);
        return count;
}

static int __init test_hello_init(void)
{
	pr_info("%s: In init\n", __func__);

	my_attr.attr.name = "my_file";
	my_attr.attr.mode = 0666;
	my_attr.show = my_show;
	my_attr.store = my_store;

	if (sysfs_create_file(kernel_kobj, &my_attr.attr)) {
		pr_err("unable to create sysfs file\n");
		return  -1;
	}
	pr_info("sysfs file created successfully\n");
	return 0;
}

static void __exit test_hello_exit(void)
{
	pr_info("%s: In exit\n", __func__);
	sysfs_remove_file(kernel_kobj, &my_attr.attr);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
