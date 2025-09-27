#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

/* same as previous, but waiting to receive 20 bytes.
when we recive so, it wakes up */
/* it will only wakeup after reciving 20 bytes:
echo "hellohellohellohello" > /dev/mydevice
it less characters are passed, then it will schedule itself again because signal will not be handled properly and received characters will be less */

int base_minor = 0;
char *device_name = "mychardev";
int count = 1;
int write_count = 0;
dev_t devicenumber;

static struct class *class = NULL;
static struct device *device = NULL;
static struct cdev *mycdev = NULL;

struct task_struct *sleeping_task = NULL;

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
	do {
		set_current_state(TASK_INTERRUPTIBLE);
		sleeping_task = current;
		pr_info("%s: calling schedule\n", __func__);
		schedule();
		pr_info("%s: after schedule\n", __func__);
	}while(write_count < 20);
	return 0;
}

static ssize_t device_write(struct file *file, const char __user *user_buffer,
                       size_t count, loff_t *offset)
{
	pr_info("%s\n", __func__);
	write_count = count;
	wake_up_process(sleeping_task);
        return count;
}

struct file_operations device_fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

static int test_hello_init(void)
{
	class = class_create(THIS_MODULE, "myclass");

	if (!alloc_chrdev_region(&devicenumber, base_minor, count, device_name)) {
		printk("Device number registered\n");
		printk("Major number received:%d\n", MAJOR(devicenumber));

		device = device_create(class, NULL, devicenumber, NULL, "mydevice");
		mycdev = cdev_alloc();
		if (mycdev) {
			mycdev->ops = &device_fops;
			mycdev->owner = THIS_MODULE;
			cdev_add(mycdev, devicenumber, count);
		}

	}
	else
		printk("Device number registration Failed\n");

	return 0;
}

static void test_hello_exit(void)
{
	device_destroy(class, devicenumber);
        class_destroy(class);
	cdev_del(mycdev);
	unregister_chrdev_region(devicenumber, count);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
