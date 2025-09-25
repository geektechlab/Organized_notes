#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include "ioctl_cmd.h"

/* echo '0' > /proc/linux/debug */

#define COMPANY_NAME	"linux"

MODULE_LICENSE("GPL");
static struct proc_dir_entry *dir_pde, *file_pde; //pde -proc directory entry
unsigned int debug_level = 0;

static int debug_show(struct seq_file *m, void *v)
{
        seq_printf(m, "%u\n", debug_level);
        return 0;
}

static int my_open(struct inode *inode, struct file *file)
{
	return single_open(file, debug_show, NULL);
}

static ssize_t proc_write_debug(struct file * file, const char *buffer, size_t count, loff_t * data)
{
	unsigned char buf[10];

	pr_info("%s\n", __func__);
	if (copy_from_user(buf, buffer, count))
	{
		return -EFAULT;
	}
	else
	{
		buf[count] = '\0';
		debug_level = simple_strtoul(buf, NULL, 10);
	}

	return count;
}

static struct file_operations debug_ops = {
  .owner = THIS_MODULE,
  .open  = my_open,
  .read = seq_read,
  .llseek= seq_lseek,
  .write = proc_write_debug,
};

#define DEVICE_NAME	"msg"
#define MAX_SIZE        1024
char kernel_buffer[MAX_SIZE];
int buffer_index;
MODULE_LICENSE("GPL");

static int device_open(struct inode *inode, struct file *file)
{
	if (debug_level > 0)
		pr_info("%s\n", __func__);
	file->f_pos = 0;
	buffer_index = 0;
	return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
	if (debug_level > 0)
		pr_info("%s\n", __func__);
        return 0;
}

static ssize_t device_read(struct file *file, char __user *user_buffer,
                      size_t read_count, loff_t *offset)
{
	int bytes_read;
	int available_space;
	int bytes_to_read;

	if (debug_level > 0)
		pr_info("%s read offset:%lld\n", __func__, *offset);
	 available_space = MAX_SIZE - *(offset);

	if (read_count < available_space)
		bytes_to_read = read_count;
	else
		bytes_to_read = available_space;
	
	if (debug_level > 0)
		pr_info("bytes_to_read:%d\n", bytes_to_read);

	if (bytes_to_read == 0) {
		pr_err("%s: No available space in the buffer for reading\n",
				__func__);
		return -ENOSPC;
	}

	if (buffer_index > *offset)
                bytes_to_read = buffer_index - *offset;
        else
                return 0;

	bytes_read = bytes_to_read - copy_to_user(user_buffer, kernel_buffer+*offset, bytes_to_read);
	if (debug_level > 0)
		pr_info("%s: Copy to user returned:%d\n", __func__, bytes_to_read);

	//update file offset
	*offset += bytes_read;

        return bytes_read;
}

static ssize_t device_write(struct file *file, const char __user *user_buffer,
                       size_t write_count, loff_t *offset)
{
	int bytes_written;
	int available_space;
	int bytes_to_write;

	if (debug_level > 0)
		pr_info("%s write offset:%lld\n", __func__, *offset);
	available_space = MAX_SIZE - *(offset);

	if (write_count < available_space)
		bytes_to_write = write_count;
	else
		bytes_to_write = available_space;

	if (bytes_to_write == 0) {
		pr_err("%s: No available space in the buffer for writing\n",
				__func__);
		return -ENOSPC;
	}

	bytes_written = bytes_to_write - copy_from_user(kernel_buffer+*offset,  user_buffer, bytes_to_write);

	if (debug_level > 0){
		pr_info("%s: Bytes written:%d\n", __func__, bytes_written);
		pr_info("%s: kernel_buffer:%s\n", __func__, kernel_buffer);
	}
	//update file offset
	*offset += bytes_written;
	buffer_index += bytes_written;
        return bytes_written;
}

static loff_t device_lseek(struct file *file, loff_t offset, int orig)
{
        loff_t new_pos = 0;

        switch(orig) {
                case 0 : /*seek set*/
                        new_pos = offset;
                        break;
                case 1 : /*seek cur*/
                        new_pos = file->f_pos + offset;
                        break;
                case 2 : /*seek end*/
                        new_pos = MAX_SIZE - offset;
                        break;
        }
        if(new_pos > MAX_SIZE)
                new_pos = MAX_SIZE;
        if(new_pos < 0)
                new_pos = 0;
        file->f_pos = new_pos;
        return new_pos;
}

long device_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	unsigned char ch;
	int retval = 0;
	long size = _IOC_SIZE(cmd);

	if (debug_level > 0)
		pr_info("%s: Cmd:%u\t Arg:%lu Size:%lu add:%p\n", __func__, cmd, arg, size, &ch);

	if (_IOC_TYPE(cmd) != MSG_MAGIC_NUMBER) return -ENOTTY;
	if (_IOC_NR(cmd) > MSG_IOCTL_MAX_CMDS) return -ENOTTY;

	//access_ok is kernel-oriented, so the concept of read and write is reversed
	
	retval = access_ok((void __user *)arg, size);

	if (debug_level > 0)
		pr_info("access_ok returned:%d\n", retval);
	if (!retval)
		return -EFAULT;

	switch(cmd)
	{
		//Get Length of buffer
		case MSG_IOCTL_GET_LENGTH:
			if (debug_level > 0)
				pr_info("Get Buffer Length\n");
			put_user(MAX_SIZE, (unsigned int *)arg);
			break;
		//clear buffer
		case MSG_IOCTL_CLEAR_BUFFER:
			if (debug_level > 0)
				pr_info("Clear buffer\n");
			memset(kernel_buffer, 0, sizeof(kernel_buffer));
			break;
		//fill character
		case MSG_IOCTL_FILL_BUFFER:
			get_user(ch, (unsigned char *)arg);
			if (debug_level > 0)
				pr_info("Fill Character:%c\n", ch);
			memset(kernel_buffer, ch, sizeof(kernel_buffer));
			buffer_index = sizeof(kernel_buffer);
			break;
		//address of kernel buffer
		case MSG_GET_ADDRESS:
			put_user(0x12345678, (unsigned long*)arg);
			if (debug_level > 0)
				pr_info("MSG_GET_ADDRESS\n");
			break;
		default:
			if (debug_level > 0)
				pr_info("Unknown Command:%u\n", cmd);
			return -ENOTTY;
	}
	return 0;
}


long device_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	unsigned char ch;
	int retval = 0;
	long size = _IOC_SIZE(cmd);
	unsigned int tmp_cmd = _IOR(MSG_MAGIC_NUMBER, 4, u32);

	if (debug_level > 0)
		pr_info("%s: Cmd:%u\t Arg:%lu Size:%lu tm_cmd:%u\n", __func__, cmd, arg, size, tmp_cmd);


	if (_IOC_TYPE(cmd) != MSG_MAGIC_NUMBER) return -ENOTTY;
	if (_IOC_NR(cmd) > MSG_IOCTL_MAX_CMDS) return -ENOTTY;

	if (cmd == tmp_cmd)
		cmd = MSG_GET_ADDRESS;

	//access_ok is kernel-oriented, so the concept of read and write is reversed
	
	retval = access_ok((void __user *)arg, size);

	if (debug_level > 0)
		pr_info("access_ok returned:%d\n", retval);
	if (!retval)
		return -EFAULT;

	switch(cmd)
	{
		//Get Length of buffer
		case MSG_IOCTL_GET_LENGTH:
			if (debug_level > 0)
				pr_info("Get Buffer Length\n");
			put_user(MAX_SIZE, (unsigned int *)arg);
			break;
		//clear buffer
		case MSG_IOCTL_CLEAR_BUFFER:
			if (debug_level > 0)
				pr_info("Clear buffer\n");
			memset(kernel_buffer, 0, sizeof(kernel_buffer));
			break;
		//fill character
		case MSG_IOCTL_FILL_BUFFER:
			get_user(ch, (unsigned char *)arg);
			if (debug_level > 0)
				pr_info("Fill Character:%c\n", ch);
			memset(kernel_buffer, ch, sizeof(kernel_buffer));
			buffer_index = sizeof(kernel_buffer);
			break;
		//address of kernel buffer
		case MSG_GET_ADDRESS:
			put_user(0x12345678, (unsigned long*)arg);
			if (debug_level > 0)
				pr_info("MSG_GET_ADDRESS\n");
			break;
		default:
			if (debug_level > 0)
				pr_info("Unknown Command:%u\n", cmd);
			return -ENOTTY;
	}
	return 0;
}

struct file_operations device_fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release,
	.llseek = device_lseek,
	.unlocked_ioctl = device_ioctl,
	.compat_ioctl = device_compat_ioctl
};

static struct miscdevice my_misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &device_fops
};

static int hello_init(void)
{
	pr_info("%s: In init\n", __func__);
	dir_pde = proc_mkdir(COMPANY_NAME, NULL);
	if (!dir_pde) {
		pr_err("%s: error creating proc_dir\n", __func__);
		return -1;
	}
	pr_info("Proc directory created\n");

	file_pde = proc_create("debug",0644, dir_pde, &debug_ops);
	if (!file_pde) {
		pr_err("%s: error creating proc file\n", __func__);
		return -1;
	}
	if (misc_register(&my_misc_device)) {
		pr_err("Couldn't register device misc, "
				"%d.\n", my_misc_device.minor);
		return -EBUSY;
	}
	pr_info("Succeeded in registering character device %s\n", DEVICE_NAME);


	return 0;
}

static void hello_exit(void)
{
	pr_info("%s: In exit\n", __func__);
	remove_proc_entry("debug", dir_pde);
	remove_proc_entry(COMPANY_NAME, NULL);
	misc_deregister(&my_misc_device);
}

module_init(hello_init);
module_exit(hello_exit);
