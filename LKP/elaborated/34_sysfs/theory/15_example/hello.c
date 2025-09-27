#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

MODULE_LICENSE("GPL");

static int my_value = 0;
static struct kobject *my_kobj;

static ssize_t my_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf)
{
	return sprintf(buf, "%d\n", my_value);
}

static ssize_t my_store(struct  kobject *kobj, struct kobj_attribute *attr,
                        const char *buf,size_t count)
{
	int ret;

	ret = kstrtoint(buf, 10, &my_value);
        if (ret < 0)
                return ret;

        return count;
}

static struct kobj_attribute my_attr = __ATTR(my_file, 0644, my_show, my_store);

static int __init test_hello_init(void)
{
	pr_info("%s: In init\n", __func__);

	my_kobj = kobject_create_and_add("my_kobj", NULL);
        if (!my_kobj)
                return -ENOMEM;

	if (sysfs_create_file(my_kobj, &my_attr.attr)) {
		pr_err("unable to create sysfs file\n");
		return  -1;
	}
	pr_info("sysfs file created successfully\n");
	return 0;
}

static void __exit test_hello_exit(void)
{
	pr_info("%s: In exit\n", __func__);
	kobject_put(my_kobj);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
