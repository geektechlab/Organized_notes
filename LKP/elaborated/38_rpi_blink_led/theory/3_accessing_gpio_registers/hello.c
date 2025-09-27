#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/io.h>

MODULE_LICENSE("GPL");

#define GPIO_BASE               0x3f200000      // GPIO controller 
#define GPIO_SIZE               0xb4

void *mem;

static int led_init(void)
{
    pr_info("%s: In init\n", __func__);
    mem = ioremap(GPIO_BASE, GPIO_SIZE);
    pr_info("ioremap returned:%px\n", mem);
    iounmap(mem);
    return 0;
}

static void led_exit(void)
{
    pr_info("%s: In exit\n", __func__);
}

module_init(led_init);
module_exit(led_exit);
