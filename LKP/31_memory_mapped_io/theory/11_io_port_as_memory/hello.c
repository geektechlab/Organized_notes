#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

#define MY_BASEPORT 0x0061
#define NUMBER_PORTS 1
static u8 __iomem *base_iomem;

static int mod_init(void)
{

	if (!request_region(MY_BASEPORT, NUMBER_PORTS, "myport")) {
                pr_info("request region failed for myport\n");
        } else {
                pr_info("request region success for myport\n");
		base_iomem = ioport_map(MY_BASEPORT, NUMBER_PORTS);
		iowrite8(0x03, base_iomem);
		ssleep(5);
		iowrite8(0x00, base_iomem);
        }
	return 0;
}

static void mod_exit(void)
{
	ioport_unmap(base_iomem);
        release_region(MY_BASEPORT, NUMBER_PORTS);

}

module_init(mod_init);
module_exit(mod_exit);
