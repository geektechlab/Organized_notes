#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/seq_file.h>

int  buffer[] = {10, 20, 30, 40 ,50};
MODULE_LICENSE("GPL");

static struct proc_dir_entry *dir_entry;

/**
 * This function is called at the beginning of a sequence.
 * ie, when:
 *	- the /proc file is read (first time)
 *	- after the function stop (end of sequence)
 *
 */
static void *my_seq_start(struct seq_file *s, loff_t *pos)
{
	pr_info("%s\n", __func__);
	pr_info("%s: offset:%llu\n", __func__, *pos);

	//beginning a new sequence
	if (*pos == 0) 
		//return a non null value to begin the sequence
		return &buffer[*pos];
	else
	{
		//it's the end of the sequence, return end to stop reading 
		*pos = 0;
		return NULL;
	}
}

/**
 * This function is called after the beginning of a sequence.
 * It's called untill the return is NULL (this ends the sequence).
 *
 */
static void *my_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	if (v!= NULL)
		pr_info("%s pos:%llu v:%d\n", __func__, *pos, *(int *)v);	
	else
		pr_info("%s pos:%llu\n", __func__, *pos);	
	
	if (*pos == 4)
		return NULL;
	else
		return &buffer[++*pos];
}

/**
 * This function is called at the end of a sequence
 *
 */
static void my_seq_stop(struct seq_file *s, void *v)
{
	pr_info("%s\n", __func__);
}

/**
 * This function is called for each "step" of a sequence
 *
 */
static int my_seq_show(struct seq_file *s, void *v)
{
	int *buff = (int *)v;

	pr_info("%s v:%d\n", __func__, *(int *)v);
	seq_printf(s, "%d", *buff);
	return 0;
}

static struct seq_operations my_seq_ops = {
	.start = my_seq_start,
	.next  = my_seq_next,
	.stop  = my_seq_stop,
	.show  = my_seq_show
};

static int my_open(struct inode *inode, struct file *filp)
{
	return seq_open(filp, &my_seq_ops);
}

static struct file_operations proc_ops = 
{
	.owner = THIS_MODULE,
	.open = my_open, //user supplied
	.read = seq_read, //interface supplied
	.release = seq_release, //interface supplied
	.llseek = seq_lseek //interface supplied
};

static int proc_init(void)
{
	dir_entry = proc_mkdir("embedded", NULL);

	proc_create("ldd",0646,dir_entry,&proc_ops);
	pr_info("ldd proc entry created\n");
	return 0;
}

static void proc_cleanup(void)
{
	remove_proc_entry("ldd", dir_entry);
	remove_proc_entry("embedded", NULL);
}

module_init(proc_init);
module_exit(proc_cleanup);
