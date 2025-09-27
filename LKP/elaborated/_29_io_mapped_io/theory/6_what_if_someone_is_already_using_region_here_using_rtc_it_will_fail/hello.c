#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioport.h>

/* check updated io region through [ cat /proc/ioport ] */

#define MY_BASEPORT 0x0070
#define NUMBER_PORTS 2

MODULE_LICENSE("GPL");

static int mod_init(void)
{
	if (!request_region(MY_BASEPORT, NUMBER_PORTS, "myports")) {
		pr_info("request region failed for myports\n");
		return -1;
	} else {
		pr_info("request region success for myports\n");
	}
	return 0;
}

static void mod_exit(void)
{
	release_region(MY_BASEPORT, NUMBER_PORTS);
}

module_init(mod_init);
module_exit(mod_exit);
