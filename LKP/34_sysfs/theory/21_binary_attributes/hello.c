#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

MODULE_LICENSE("GPL");

static struct kobject *my_kobj;
struct bin_attribute my_bin_attr;

unsigned char smbios_entry_point[] = {
  0x5f, 0x53, 0x4d, 0x5f, 0x07, 0x1f, 0x02, 0x07, 0x73, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x5f, 0x44, 0x4d, 0x49, 0x5f, 0xe7, 0x84, 0x71,
  0x10, 0x00, 0x0e, 0x00, 0x6c, 0x02, 0x00
};
unsigned int smbios_entry_point_len = 31;

static ssize_t my_bin_attr_read(struct file *filp, struct kobject *kobj,
                struct bin_attribute *attr, char *buf, loff_t off, size_t count)
{
	memcpy(buf, smbios_entry_point, smbios_entry_point_len);
	return smbios_entry_point_len;
}

static int __init test_hello_init(void)
{
	pr_info("%s: In init\n", __func__);

	my_kobj = kobject_create_and_add("my_kobj", NULL);
        if (!my_kobj)
                return -ENOMEM;
	sysfs_bin_attr_init(&my_bin_attr);
	my_bin_attr.attr.name = "my_bin_file";
	my_bin_attr.attr.mode = S_IRUSR | S_IWUSR;
	my_bin_attr.read = my_bin_attr_read;
	my_bin_attr.size = smbios_entry_point_len;

	if (sysfs_create_bin_file(my_kobj, &my_bin_attr)) {
		pr_err("Bin file failed to create\n");
	} else {
		pr_info("Bin file created successfully\n");
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
