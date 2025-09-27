#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>

#define PIT_MODE        0x43
#define PIT_CH0         0x40
#define PIT_CH2         0x42
#define PIT_TICK_RATE 1193182ul
#define PIT_MASK 0xFF

static int __init test_hello_init(void)
{
	int divisor = PIT_TICK_RATE / 100;
	unsigned char pic = inb_p(0x21);
	pr_info("pic:%02x\n", pic);
	outb_p(0xfe, 0x21); //enabling PIT
	outb_p(0x46, PIT_MODE);
	outb_p(divisor & PIT_MASK, PIT_CH0);      /* LSB */
        outb_p((divisor >> 8)&PIT_MASK, PIT_CH0);        /* MSB */

	return 0;
}

static void __exit test_hello_exit(void)
{
}

MODULE_LICENSE("GPL");

module_init(test_hello_init);
module_exit(test_hello_exit);
