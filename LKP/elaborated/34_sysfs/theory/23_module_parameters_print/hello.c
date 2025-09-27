#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/sysfs.h>

int base_minor = 0;
char *device_name = "mychardev";
int count = 1;
dev_t devicenumber;

static struct class *class = NULL;
static struct device *device = NULL;
static struct cdev mycdev;

MODULE_LICENSE("GPL");

static int device_open(struct inode *inode, struct file *file)
{
	pr_info("%s\n", __func__);
	return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
	pr_info("%s\n", __func__);
        return 0;
}

static ssize_t device_read(struct file *file, char __user *user_buffer,
                      size_t count, loff_t *offset)
{
	pr_info("%s\n", __func__);
        return 0;
}

static ssize_t device_write(struct file *file, const char __user *user_buffer,
                       size_t count, loff_t *offset)
{
	pr_info("%s\n", __func__);
        return count;
}

struct file_operations device_fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

static int my_value = 0;

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

static int test_hello_init(void)
{
	class = class_create(THIS_MODULE, "myclass");

	if (!alloc_chrdev_region(&devicenumber, base_minor, count, device_name)) {
		printk("Device number registered\n");
		printk("Major number received:%d\n", MAJOR(devicenumber));

		device = device_create(class, NULL, devicenumber, NULL, "mydevice");
		cdev_init(&mycdev, &device_fops);
		mycdev.owner = THIS_MODULE;
		cdev_add(&mycdev, devicenumber, count);
		pr_info("dev->kobj->name:%s\n", device->kobj.name);
		pr_info("dev->kobj->parent->name:%s\n", device->kobj.parent->name);
		pr_info("dev->kobj->parent->name:%s\n", device->kobj.parent->parent->name);
		pr_info("dev->kobj->parent->parent->name:%s\n", device->kobj.parent->parent->parent->name);
		if (sysfs_create_file(&device->kobj, &my_attr.attr)) {
			pr_err("unable to create sysfs file\n");
			return  -1;
		}
		pr_info("sysfs file created successfully\n");

	}
	else
		printk("Device number registration Failed\n");

	return 0;
}

static void test_hello_exit(void)
{
	sysfs_remove_file(kernel_kobj, &my_attr.attr);
	device_destroy(class, devicenumber);
	class_destroy(class);
	cdev_del(&mycdev);
	unregister_chrdev_region(devicenumber, count);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
