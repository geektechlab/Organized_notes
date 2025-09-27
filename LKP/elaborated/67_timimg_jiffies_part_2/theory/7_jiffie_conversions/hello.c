#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/delay.h>

static int __init test_hello_init(void)
{
	struct timespec timespec;                                                                                
	struct timeval timeval;
	unsigned long tmp=0;
	
	tmp = jiffies;
	jiffies_to_timespec(tmp,&timespec);                                                                      
	jiffies_to_timeval(tmp,&timeval);

	pr_info("jiffies=%lu",tmp);                                                                
	pr_info("timespec %ld s .%ld ns",timespec.tv_sec,timespec.tv_nsec);	                                 
	pr_info("timeval  %ld s .%ld us\n",timeval.tv_sec,timeval.tv_usec);          	

	pr_info("timespec_to_jiffies:%lu\n", timespec_to_jiffies(&timespec));
	pr_info("timeval_to_jiffies:%lu\n", timeval_to_jiffies(&timeval));

	return -1;
}

static void __exit test_hello_exit(void)
{
}

MODULE_LICENSE("GPL");

module_init(test_hello_init);
module_exit(test_hello_exit);
