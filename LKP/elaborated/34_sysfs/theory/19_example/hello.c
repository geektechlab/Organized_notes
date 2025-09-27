#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

MODULE_LICENSE("GPL");

static int my_value1 = 0;
static int my_value2 = 0;
static struct kobject *my_kobj;

static ssize_t my_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf)
{
	pr_info("attribute name:%s\n", attr->attr.name);
	if (strcmp(attr->attr.name, "my_file1") == 0)
		return sprintf(buf, "%d\n", my_value1);
	else
		return sprintf(buf, "%d\n", my_value2);
}

static ssize_t my_store(struct  kobject *kobj, struct kobj_attribute *attr,
                        const char *buf,size_t count)
{
	int ret, tmp;

	pr_info("attribute name:%s\n", attr->attr.name);
	ret = kstrtoint(buf, 10, &tmp);
        if (ret < 0)
                return ret;

	if (strcmp(attr->attr.name, "my_file1") == 0)
		my_value1 = tmp;
	else
		my_value2 = tmp;
        return count;
}

static struct kobj_attribute my_attr1 = __ATTR(my_file1, 0644, my_show, my_store);
static struct kobj_attribute my_attr2 = __ATTR(my_file2, 0644, my_show, my_store);

/*
 * Create a group of attributes so that we can create and destroy them all
 * at once.
 */
static struct attribute *attrs[] = {
        &my_attr1.attr,
        &my_attr2.attr,
        NULL,   /* need to NULL terminate the list of attributes */
};

//If we specify a name, a subdirectory will be created for the attributes with the directory being the name of the//attribute group.

static struct attribute_group attr_group = {
	.name = "mygroup", 
        .attrs = attrs,
};

static int __init test_hello_init(void)
{
	pr_info("%s: In init\n", __func__);

	my_kobj = kobject_create_and_add("my_kobj", NULL);
        if (!my_kobj)
                return -ENOMEM;

	if (sysfs_create_group(my_kobj, &attr_group)) {
		pr_err("Failed to create sysfs group\n");
	} else {
		pr_info("sysfs files created successfully\n");
	}
	return 0;
}

static void __exit test_hello_exit(void)
{
	pr_info("%s: In exit\n", __func__);
	kobject_put(my_kobj);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
