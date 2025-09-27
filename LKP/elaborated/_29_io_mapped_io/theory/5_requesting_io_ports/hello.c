#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioport.h>

#define MY_BASEPORT 0x0062
#define NUMBER_PORTS 2

MODULE_LICENSE("GPL");

static int mod_init(void)
{
	if (!request_region(MY_BASEPORT, NUMBER_PORTS, "myports")) {
		pr_info("request region failed for myports\n");
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
