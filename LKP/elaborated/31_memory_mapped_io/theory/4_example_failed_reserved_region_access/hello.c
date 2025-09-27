#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioport.h>

#define MY_BASEADDRESS	0x00000000
#define LENGTH		0xf

MODULE_LICENSE("GPL");

static int mod_init(void)
{
	if (!request_mem_region(MY_BASEADDRESS, LENGTH, "myports")) {
		pr_info("request mem region failed for myports\n");
		return -1;
	} else {
		pr_info("request mem region success for myports\n");
	}
	return 0;
}

static void mod_exit(void)
{
	release_mem_region(MY_BASEADDRESS, LENGTH);
}

module_init(mod_init);
module_exit(mod_exit);
