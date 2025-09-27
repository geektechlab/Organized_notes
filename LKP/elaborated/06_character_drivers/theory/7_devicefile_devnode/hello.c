#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/cdev.h>

/* after inserting this module, run [ ls /sys/class/myclasss -al ] and check mydevice1 and mydevice2 gets created */
/* also run [ ls /dev/mydevice* -al ] and check there also /dev/mydevice1 and /dev/mydevice2 gets created. Also check their major and minor numbers */
/* also check this after removing this module */

MODULE_LICENSE("GPL");
static struct class *class;
static struct device *device1, *device2;
dev_t dev1, dev2;

static int test_hello_init(void)
{
    dev1 = MKDEV(200, 1);
    dev2 = MKDEV(200, 2);

    class = class_create(THIS_MODULE, "myclass");
    device1 = device_create(class, NULL, dev1, NULL, "mydevice%d", 1);
    device2 = device_create(class, NULL, dev2, NULL, "mydevice%d", 2);

    return 0;
}

static void test_hello_exit(void)
{
	device_destroy(class, dev1);
	device_destroy(class, dev2);
	class_destroy(class);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
