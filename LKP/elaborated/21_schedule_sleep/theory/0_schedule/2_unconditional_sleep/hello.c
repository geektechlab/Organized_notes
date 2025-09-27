#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>               //task_struct 
#include <linux/delay.h>


 
static int __init my_driver_init(void)
{
	pr_info("%s: processor id :%d before schedule\n", __func__, smp_processor_id());
	set_current_state(TASK_INTERRUPTIBLE);
	schedule();
	pr_info("%s: processor id:%d after schedule\n", __func__, smp_processor_id());
	return 0;
}
 
void __exit my_driver_exit(void)
{
        pr_info("%s:Device Driver Remove...Done\tProcessor Id:%d\n", __func__, smp_processor_id());
}
 
module_init(my_driver_init);
module_exit(my_driver_exit);
 
MODULE_LICENSE("GPL");
