#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/delay.h>
#include <asm/io.h>

#define RTC_BASEADDRESS	0x101e8000
#define LENGTH		0xf

MODULE_LICENSE("GPL");

static int mod_init(void)
{
	void __iomem *p;
	u32 seconds;

	p = ioremap(RTC_BASEADDRESS, LENGTH);
	pr_info("ioremap returned:%px\n", p);

	//read data register which increments every second
	seconds = ioread32(p);
	pr_info("seconds:%04x\n", seconds);
	ssleep(5);
	seconds = ioread32(p);
	pr_info("seconds:%04x\n", seconds);

	return 0;
}

static void mod_exit(void)
{
}

module_init(mod_init);
module_exit(mod_exit);
