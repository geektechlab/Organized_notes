#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

static int mod_init(void)
{
	outb(0x03, 0x0061); //Enabling speaker
	ssleep(5);
	outb(0x00, 0x0061); //Disabling Speaker
	return 0;
}

static void mod_exit(void)
{
}

module_init(mod_init);
module_exit(mod_exit);

