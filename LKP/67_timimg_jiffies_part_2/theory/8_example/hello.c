#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/delay.h>

static int __init test_hello_init(void)
{
	struct timespec timespec;                                                                                
	struct timeval timeval;
	unsigned long tmp1 = 0;
	unsigned long tmp2 = 0;

	tmp1 = jiffies;
	jiffies_to_timespec(tmp1,&timespec);                                                                      
	jiffies_to_timeval(tmp1,&timeval);

	pr_info("jiffies=%lu",tmp1);                                                                
	pr_info("timespec %ld s .%ld ns",timespec.tv_sec,timespec.tv_nsec);	                                 
	pr_info("timeval  %ld s .%ld us\n",timeval.tv_sec,timeval.tv_usec);          	

	mdelay(140);

	tmp2 = jiffies;
	jiffies_to_timespec(tmp2,&timespec);                                                                      
	jiffies_to_timeval(tmp2,&timeval);

	pr_info("jiffies=%lu",tmp2);                                                                
	pr_info("timespec %ld s .%ld ns",timespec.tv_sec,timespec.tv_nsec);	                                 
	pr_info("timeval  %ld s .%ld us\n",timeval.tv_sec,timeval.tv_usec);          	

	return -1;
}

static void __exit test_hello_exit(void)
{
}

MODULE_LICENSE("GPL");

module_init(test_hello_init);
module_exit(test_hello_exit);
