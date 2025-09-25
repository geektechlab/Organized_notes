#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

#define GPIO_BASE               0x3f200000      // GPIO controller 
#define GPIO_SIZE               0xb4

#define GPFSEL0_OFFSET		0x00
#define GPSET0_OFFSET		0x07
#define GPCLR0_OFFSET		0x0A

uint32_t *mem;

void set_gpio_output(unsigned int gpio)
{
	int register_index = gpio/10;
	int bit_location   = (gpio%10)*3;
	uint32_t mask = 7 << bit_location;
	uint32_t old_value;
	uint32_t new_value;

	old_value = ioread32(mem + GPFSEL0_OFFSET + register_index);
	new_value =  (old_value & ~mask) | ((0b001 << bit_location) &mask);
	iowrite32(new_value, mem + GPFSEL0_OFFSET + register_index);

}

static void set_gpio(unsigned int gpio)
{
	int register_index = gpio/32;
	unsigned int value = (1 << (gpio % 32));
    
	iowrite32(value, mem + GPSET0_OFFSET + register_index);
}

static void clr_gpio(unsigned int gpio)
{
	int register_index = gpio/32;
	unsigned int value = (1 << (gpio % 32));
	
	iowrite32(value, mem + GPCLR0_OFFSET + register_index);
}

static int led_init(void)
{
    pr_info("%s: In init\n", __func__);
    mem = (uint32_t *)ioremap(GPIO_BASE, GPIO_SIZE);
    pr_info("ioremap returned:%px\n", mem);
    set_gpio_output(14);
    set_gpio(14);
    ssleep(1);
    clr_gpio(14);
    iounmap(mem);
    return 0;
}

static void led_exit(void)
{
    pr_info("%s: In exit\n", __func__);
}

module_init(led_init);
module_exit(led_exit);
