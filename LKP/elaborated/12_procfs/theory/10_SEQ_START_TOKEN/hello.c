#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/seq_file.h>
#include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/sched/signal.h>
 
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
	loff_t index = *pos;
	if (index == 0) {
	//	return &init_task;
		return SEQ_START_TOKEN;
	}
	else {
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
	struct task_struct *task = (struct task_struct *)v;

	if (*pos == 0)
		task = &init_task;
	else 
		task = next_task(task);
	if ((*pos != 0) && (task == &init_task)) {
		return NULL;
	}
	++*pos;
	return task;
}

/**
 * This function is called at the end of a sequence
 *
 */
static void my_seq_stop(struct seq_file *s, void *v)
{
}

/**
 * This function is called for each "step" of a sequence
 *
 */
static int my_seq_show(struct seq_file *s, void *v)
{
	struct task_struct * task = (struct task_struct *)v;

	if (v == SEQ_START_TOKEN) {
		seq_printf(s, "Current all the processes in system:\n"
				"%-24s%-5s\n", "name", "pid");
		return 0;
	}

	seq_printf(s, "%-24s%-5d\n", task->comm, task->pid);
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

	proc_create("pid",0640,dir_entry,&proc_ops);
	pr_info("ldd proc entry created\n");
	return 0;
}

static void proc_cleanup(void)
{
	remove_proc_entry("pid", dir_entry);
	remove_proc_entry("embedded", NULL);
}

module_init(proc_init);
module_exit(proc_cleanup);
